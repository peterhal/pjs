<?hh //strict

class ParseTree
{
  public function __construct(
    private Range $range,
    private ParseTreeKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): ParseTreeKind { return $this->kind; }
}
