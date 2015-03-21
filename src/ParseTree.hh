<?hh //strict

require_once 'ParseTreeKind.hh';

class ParseTree
{
  public function __construct(
    private Range $range,
    private ParseTreeKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): ParseTreeKind { return $this->kind; }
  public function isScript(): bool {
    return $this->kind === ParseTreeKind::SCRIPT;
  }
  public function asScript(): ScriptTree { 
    invariant($this instanceof ScriptTree, "Wrong type.");
    return $this;
  }
}
