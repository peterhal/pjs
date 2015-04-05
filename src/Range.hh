<?hh //strict

require_once 'Location.hh';

namespace Utils {

class Range
{
  public function __construct(
    private Location $start,
    private Location $end)
  {}

  public function file(): SourceFile { return $this->start->file(); }
  public function start(): Location { return $this->start; }
  public function end(): Location { return $this->end; }
  public function length(): int { 
    return $this->end->offset() - $this->start->offset(); 
  }
  public function text(): string {
    return substr(
      $this->file()->contents(),
      $this->start->offset(),
      $this->length());
  }
}

}
