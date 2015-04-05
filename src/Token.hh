<?hh //strict

require_once 'Range.hh';
require_once 'Location.hh';

namespace Syntax {

use Utils\Location;
use Utils\Range;

class Token
{
  public function __construct(
    private Range $range,
    private TokenKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): TokenKind { return $this->kind; }
  public function start(): Location { return $this->range->start(); }
  public function end(): Location { return $this->range->end(); }
  public function text(): string { return $this->range->text(); }

  public function isName(): bool
  {
    return $this->kind === TokenKind::NAME;
  }

  public function asName(): NameToken
  {
    invariant($this instanceof NameToken, "Must be a name.");
    return $this;
  }

  public function isVariableName(): bool
  {
    return $this->kind === TokenKind::VARIABLE_NAME;
  }

  public function asVariableName(): VariableNameToken
  {
    invariant($this instanceof VariableNameToken, "Must be a variable name.");
    return $this;
  }

  public function isLiteral(): bool
  {
    switch ($this->kind) {
    case TokenKind::NUMBER:
    case TokenKind::SINGLE_QUOTED_STRING:
    case TokenKind::DOUBLE_QUOTED_STRING:
      return true;
    default:
     return false;
    }
  }

  // Precedes immediately with no characters separating.
  public function precedes(Token $other): bool
  {
    return $this->end()->isEqual($other->start());
  }
}

class NameToken extends Token
{
  public function __construct(
    Range $range,
    private string $value)
  {
    parent::__construct($range, TokenKind::NAME);
  }
  public function value(): string { return $this->value; }
}

class VariableNameToken extends Token
{
  public function __construct(
    Range $range,
    private string $value)
  {
    parent::__construct($range, TokenKind::VARIABLE_NAME);
  }
  public function value(): string { return $this->value; }
}

}
