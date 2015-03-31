<?hh //strict

require_once 'ParseTreeKind.hh';
require_once 'Token.hh';
require_once 'TokenKind.hh';
require_once 'utils.hh';

class ParserBase
{
  protected function __construct(
      protected Vector<Token> $tokens,
      protected ErrorReporter $reporter)
  {
    $this->index = 0;
  }

  protected function eat(TokenKind $kind): Token
  {
    if ($this->peek() === $kind) {
      $result = $this->next();
      return $result;
    } else {
      // TODO: Better message.
      $this->errorLocation(
        $this->position(),
        "Expected " . TokenKind::getNames()[$kind]);
      return $this->next();
    }
  }

  protected function eatOpt(TokenKind $kind): bool
  {
    if ($this->peek() === $kind) {
      $result = $this->next();
      return true;
    } else {
      return false;
    }
  }

  protected function peekPredefinedName(string $name): bool
  {
    $peek = $this->peekToken();
    return $peek->kind() === TokenKind::NAME && 
      $peek->asName()->value() === $name;
  }

  protected function eatPredefinedName(string $name): Token
  {
    $peek = $this->peekToken();
    if ($peek->kind() !== TokenKind::NAME) {
      $this->error("Expected name '" . $name . "'.");
    } else if ($peek->asName()->value() !== $name) {
      $this->error("Expected name '" . $name
        . "'. Found '" . $peek->asName()->value() . "'.");
    }
    return $this->next();
  }

  protected function eatAnyName(): Token
  {
    if ($this->peekKind(TokenKind::VARIABLE_NAME)) {
      return $this->eat(TokenKind::VARIABLE_NAME);
    } else {
      return $this->eatName();
    }
  }

  protected function eatName(): NameToken
  {
    $token = $this->eat(TokenKind::NAME);
    if ($token instanceof NameToken) {
      return $token;
    } else {
      // TODO: String value?
      return new NameToken($token->range(), '');
    }
  }

  protected function eatVariableName(): VariableNameToken
  {
    $token = $this->eat(TokenKind::VARIABLE_NAME);
    if ($token instanceof VariableNameToken) {
      return $token;
    } else {
      // TODO: String value?
      return new VariableNameToken($token->range(), '');
    }
  }

  protected function peekTokenIndex(int $index): Token
  {
    return $this->tokens[$this->index + $index];
  }

  protected function peekToken(): Token
  {
    return $this->peekTokenIndex(0);
  }

  protected function peekKind(TokenKind $kind): bool
  {
    return $this->peek() === $kind;
  }

  protected function peekIndexKind(int $index, TokenKind $kind): bool
  {
    return $this->peekIndex($index) === $kind;
  }

  protected function peekIndex(int $index): TokenKind
  {
    return $this->peekTokenIndex($index)->kind();
  }

  protected function peek(): TokenKind
  {
    return $this->peekIndex(0);
  }

  protected function peekAny(Vector<TokenKind> $kinds): bool
  {
    return contains($kinds, $this->peek());
  }

  protected function next(): Token
  {
    $result = $this->peekToken();
    $this->index++;
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

  protected function lastPosition(): Location
  {
    return $this->tokens[$this->index - 1]->end();
  }

  protected function getRange(Location $start): Range
  {
    return new Range($start, $this->lastPosition());
  }

  protected int $index;
}
