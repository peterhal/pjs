<?hh //strict

require_once 'SourceFile.hh';

class Location
{
  public function __construct(
    private SourceFile $file,
    private int $line,
    private int $column)
  {}

  public function file(): SourceFile { return $this->file; }
  public function line(): int { return $this->line; }
  public function column(): int { return$this->column; }

  public function toString(): string
  {
    return "{$this->file()->name()}({$this->line()}, {$this->column()})";
  }

  public function isEqual(Location $other): bool
  {
    return $this->file === $other->file
      && $this->line === $other->line
      && $this->column === $other->column;
  }
}
