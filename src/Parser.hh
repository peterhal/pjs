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
    $this->eatPredefinedName(PredefinedName::hh);
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
    case TokenKind::KW_FUNCTION:
    case TokenKind::KW_ASYNC:
    case TokenKind::LEFT_SHIFT: // attributes
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
    case TokenKind::KW_ASYNC:
    case TokenKind::KW_FUNCTION:
      return $this->parseFunctionDefinition();
    case TokenKind::LEFT_SHIFT:
      // TODO: attributes
      throw new Exception();
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
    // TODO: SPEC missing ; in grammar
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
    // TODO: SPEC missing ; in grammar
    $this->eat(TokenKind::SEMI_COLON);

    return new RequireMultipleDirectiveTree(
      $this->getRange($start),
      $includeFilename);
  }

  public function parseExpression(): ParseTree
  {
    $start = $this->position();

    // TODO
    $value = $this->eat(TokenKind::SINGLE_QUOTED_STRING);

    return new LiteralTree($this->getRange($start) , $value);
  }

  public function parseFunctionDefinition(): ParseTree
  {
    $start = $this->position();

    // TODO: attributes
    // TODO: async
    $this->eat(TokenKind::KW_FUNCTION);
    $name = $this->eatName();
    // TODO: type parameters
    $parameters = $this->parseParameterList();
    $this->eat(TokenKind::COLON);
    $returnType = $this->parseReturnType();
    $body = $this->parseCompoundStatement();

    return new FunctionDefinitionTree(
      $this->getRange($start),
      $name,
      $parameters,
      $returnType,
      $body);
  }

  public function parseParameterList(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::OPEN_PAREN);

    $parameters = Vector {};
    if ($this->peekParameter()) {
      $parameters[] = $this->parseParameterDeclaration();
      while ($this->eatOpt(TokenKind::COMMA)) {
        $parameters[] = $this->parseParameterDeclaration();
      }
    }

    $this->eat(TokenKind::CLOSE_PAREN);

    return new ParameterListTree(
      $this->getRange($start),
      $parameters);
  }

  private function peekParameter(): bool
  {
    // TODO: attribute
    return $this->peekType();
  }

  private function parseParameterDeclaration(): ParseTree
  {
    $start = $this->position();

    // TODO: Attribute
    $type = $this->parseTypeSpecifier();
    $name = $this->eatVariableName();
    // TODO: default argument specifier

    return new ParameterDeclarationTree(
      $this->getRange($start),
      $type,
      $name);
  }

  public function parseCompoundStatement(): ParseTree
  {
    $start = $this->position();

    $statements = Vector {};
    // TODO: parseStatements ...
    //
    return new CompoundStatementTree(
      $this->getRange($start),
      $statements);
  }

  private function peekType(): bool
  {
    // TODO
    return true;
  }

  private function parseTypeSpecifier(): ParseTree
  {
    // TODO:
    return new CompoundStatementTree(
      $this->getRange($this->position()), 
      Vector{});
  }

  private function parseReturnType(): ParseTree
  {
    $start = $this->position();
    if ($this->peekPredefinedName(PredefinedName::this)) {
      $token = $this->eatPredefinedName(PredefinedName::this);
      return new ThisTypeTree($this->getRange($start), $token);
    }
    return $this->parseTypeSpecifier();
  }
}


