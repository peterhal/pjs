<?hh //strict

require_once 'Char.hh';
require_once 'ErrorReporter.hh';
require_once 'Location.hh';
require_once 'Range.hh';
require_once 'SourceFile.hh';

namespace Syntax {

use Utils\Char;
use Utils\ErrorReporter;
use Utils\Location;
use Utils\SourceFile;
use Utils\Range;

class LexerBase
{
  protected function __construct(
    protected SourceFile $file,
    protected ErrorReporter $reporter)
  {
    $this->offset = 0;
  }

  protected function range(int $start, int $end): Range
  {
    return new Range(
      $this->locationOfOffset($start),
      $this->locationOfOffset($end));
  }

  protected function textOfRange(int $start, int $end): string
  {
    return $this->range($start, $end)->text();
  }

  protected function currentRange(int $start): Range
  {
    return $this->range($start, $this->offset);
  }

  protected function locationOfOffset(int $offset): Location
  {
    return $this->file->location($offset);
  }

  protected function location(): Location
  {
    return $this->locationOfOffset($this->offset);
  }

  protected function next(): int
  {
    $result = $this->peek();
    $this->offset++;
    return $result;
  }

  protected function peekCharOffset(int $ch, int $offset): bool
  {
    return $this->peekOffset($offset) == $ch;
  }

  protected function peekChar(int $ch): bool
  {
    return $this->peekCharOffset($ch, 0);
  }

  protected function peek(): int
  {
    return $this->peekOffset(0);
  }

  protected function peekOffset(int $offset): int
  {
    $index = $this->offset + $offset;
    if ($index < $this->count()) {
      return ord($this->contents()[$index]);
    } else {
      return Char::EOF;
    }
  }

  protected function contents(): string
  {
    return $this->file->contents();
  }

  protected function count(): int
  {
    return $this->file->count();
  }

  protected function eof(): bool
  {
    return $this->offset >= $this->count();
  }

  protected function error(string $message): void
  {
    $this->errorOffset($this->offset, $message);
  }

  protected function errorOffset(int $offset, string $message): void
  {
    $formattedMessage = $this->locationOfOffset($offset)->toString() . ": " . $message;
    $this->reporter->error($formattedMessage);
  }

  protected int $offset;
}

}

