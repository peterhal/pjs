<?hh //strict

namespace Utils {

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
  public function displayLine(): int { return $this->line + 1; }
  public function displayColumn(): int { return$this->column + 1; }
  public function offset(): int
  {
    return $this->file->lineMap()->offsetOfLine($this->line)
      + $this->column;
  }

  public function toString(): string
  {
    return "{$this->file()->name()}({$this->displayLine()}, {$this->displayColumn()})";
  }

  public function isEqual(Location $other): bool
  {
    return $this->file === $other->file
      && $this->line === $other->line
      && $this->column === $other->column;
  }
}

}
