<?hh //strict

require_once 'ParseTreeKind.hh';
require_once 'Token.hh';
require_once 'TokenKind.hh';

class ParserBase
{
  protected function __construct(
      protected Vector<Token> $tokens,
      protected ErrorReporter $reporter)
  {
    $this->index = 0;
  }

  protected function eat(TokenKind $kind): bool
  {
    if ($this->peek() === $kind) {
      $this->next();
      return true;
    } else {
      // TODO: Better message.
      $this->error("Expected " . TokenKind::getNames()[$kind]);
      return false;
    }
  }

  protected function eatName(PredefinedName $name): bool
  {
    $peek = $this->peekToken();
    if ($peek->kind() !== TokenKind::NAME) {
      $this->error("Expected name \'" . $name . "\'.");
      return false;
    } else if ($peek->asName()->value() !== $name) {
      $this->error("Expected name \'" . $name
        . "\'. Found \'" . $peek->asName()->value() . "\'.");
      return false;
    } else {
      $this->next();
      return true;
    }
  }

  protected function peekToken(): Token
  {
    return $this->tokens[$this->index];
  }

  protected function peekKind(TokenKind $kind): bool
  {
    return $this->peek() === $kind;
  }

  protected function peek(): TokenKind
  {
    return $this->peekToken()->kind();
  }

  protected function next(): Token
  {
    $result = $this->peekToken();
    $this->index;
    return $result;
  }

  protected function eof(): bool
  {
    return $this->peekKind(TokenKind::EOF);
  }

  protected function error(string $message): void
  {
    $this->errorLocation($this->position(), $message);
  }

  protected function errorLocation(Location $position, string $message): void
  {
    $formattedMessage = $position->toString() . ": " . $message;
    $this->reporter->error($formattedMessage);
  }

  protected function position(): Location
  {
    return $this->peekToken()->start();
  }

  protected function getRange(Location $start): Range
  {
    return new Range($start, $this->position());
  }

  protected int $index;
}
