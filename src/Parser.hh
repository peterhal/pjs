<?hh //strict

require_once 'ParseTreeKind.hh';
require_once 'ParseTree.hh';
require_once 'ParserBase.hh';
require_once 'PredefinedName.hh';
require_once 'Token.hh';
require_once 'TokenKind.hh';

class Parser extends ParserBase
{
  public function __construct(
      protected Vector<Token> $tokens,
      protected ErrorReporter $reporter)
  {
    parent::__construct($tokens, $reporter);
  }

  public static function parse(
    Vector<Token> $tokens,
    ErrorReporter $reporter): ParseTree
  {
    $parser = new Parser($tokens, $reporter);
    return $parser->parseScript();
  }

  public function parseScript(): ParseTree
  {
    $start = $this->position();

    // <?hh
    $this->eat(TokenKind::OPEN_ANGLE);
    $this->eat(TokenKind::QUESTION);
    $this->eatName(PredefinedName::hh);
    // TODO: Check for whitespace

    // TODO: CHeck for //strict

    $declarations = $this->parseDeclarationList();
    return new ScriptTree(
        $this->getRange($start),
        $declarations);
  }

  public function parseDeclarationList(): Vector<ParseTree>
  {
    $result = Vector {};

    while (!$this->eof()) {
      if (!$this->peekDeclaration()) {
        $this->error("Expected declaration.");
        break;
      }
      $result[] = $this->parseDeclaration();
    }

    return $result;
  }

  public function peekDeclaration(): bool
  {
    switch ($this->peek()) {
    case TokenKind::KW_REQUIRE:
    case TokenKind::KW_REQUIRE_ONCE:
      return true;
    default:
      // TODO
      return false;
    }
  }

  public function parseDeclaration(): ParseTree
  {
    switch ($this->peek()) {
    case TokenKind::KW_REQUIRE:
      return $this->parseRequireMultipleDirective();
    case TokenKind::KW_REQUIRE_ONCE:
      return $this->parseRequireOnceDirective();
    default:
      // TODO
      throw new Exception();
    }
  }

  public function parseRequireOnceDirective(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_REQUIRE_ONCE);
    $includeFilename = $this->parseExpression();
    $this->eat(TokenKind::SEMI_COLON);

    return new RequireOnceDirectiveTree(
      $this->getRange($start),
      $includeFilename);
  }

  public function parseRequireMultipleDirective(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_REQUIRE);
    $includeFilename = $this->parseExpression();
    $this->eat(TokenKind::SEMI_COLON);

    return new RequireMultipleDirectiveTree(
      $this->getRange($start),
      $includeFilename);
  }

  public function parseExpression(): ParseTree
  {
    $start = $this->position();

    $value = $this->eat(TokenKind::SINGLE_QUOTED_STRING);
    return new LiteralTree($this->getRange($start) , $value);
  }
}


