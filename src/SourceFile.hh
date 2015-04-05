<?hh //strict

require_once 'LineMap.hh';
require_once 'Location.hh';

namespace Utils {

class SourceFile
{
  public function __construct(private string $name, private string $contents)
  {
    $this->lineMap = new LineMap($contents);
  }

  public function name(): string { return $this->name; }
  public function contents(): string { return $this->contents; }
  public function count(): int { return strlen($this->contents()); }
  public function lineMap(): LineMap { return $this->lineMap; }

  public function location(int $offset): Location
  {
    return new Location(
      $this,
      $this->lineMap->lineOfOffset($offset),
      $this->lineMap->columnOfOffset($offset));
  }

  public static function read(string $filename): SourceFile
  {
    return new SourceFile($filename, file_get_contents($filename));
  }

  private LineMap $lineMap;
}

}

