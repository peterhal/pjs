<?hh //strict

class Token
{
  public function __construct(
    private Location $location,
    private TokenKind $kind)
  {}
  public function location(): Location { return $this->location; }
  public function kind(): TokenKind { return $this->kind; }
}
