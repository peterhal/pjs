<?hh //strict

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

  public function isName(): bool
  {
    return $this->kind ===TokenKind::NAME;
  }

  public function asName(): NameToken
  {
    invariant($this instanceof NameToken, "Must be a name.");
    return $this;
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


