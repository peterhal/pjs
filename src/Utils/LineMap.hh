<?hh // strict

namespace Utils {

require_once 'Char.hh';

class LineMap
{
  public function __construct(private string $values)
  {
    $this->offsets = LineMap::createOffsets($values);
  }

  private static function createOffsets(string $values): Vector<int>
  {
    $result = Vector{ 0 };
    for ($index = 0; $index < strlen($values); $index++)
    {
      $char = ord($values[$index]);
      if (Char::isNewLine($char))
      {
        if ($char == Char::CARRIAGE_RETURN
            && $index + 1 < strlen($values)
            && $values[$index + 1] == Char::LINE_FEED)
        {
          $index++;
        }
        $result[] = $index + 1;
      }
    }
    $result[] = PHP_INT_MAX;
    return $result;
  }

  public function offsetOfLine(int $line): int
  {
    return $this->offsets[$line];
  }

  public function lineOfOffset(int $offset): int
  {
    // TODO: binary search
    for ($line = 0; $line < $this->offsets->count(); $line++) {
      if ($this->offsetOfLine($line) > $offset) {
        return $line - 1;
      }
    }

    return $line;
  }

  public function columnOfOffset(int $offset): int
  {
    return $offset - $this->offsetOfLine($this->lineOfOffset($offset));
  }

  private Vector<int> $offsets;
}

}
