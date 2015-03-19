<?hh //strict

require_once 'Location.hh';

class Range
{
  public function __construct(
    private Location $start,
    private Location $end)
  {}

  public function file(): SourceFile { return $this->start->file(); }
  public function start(): Location { return $this->start; }
  public function end(): Location { return $this->end; }
}
