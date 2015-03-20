<?hh //strict

require_once 'ParseTreeKind.hh';
require_once 'ParserBase.hh';
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

  public function parseScript(): ParseTree
  {
    $start = $this->position();

    // <?hh
    $this->eat(TokenKind::CLOSE_ANGLE);
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
    return Vector {};
  }
}


