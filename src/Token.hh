<?hh //strict

class Token
{
  public function __construct(
    private Range $range,
    private TokenKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): TokenKind { return $this->kind; }
}
