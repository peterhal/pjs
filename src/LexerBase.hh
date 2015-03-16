<?hh //strict

require_once 'Char.hh';
require_once 'ErrorReporter.hh';
require_once 'Location.hh';
require_once 'SourceFile.hh';

class LexerBase
{
  protected function __construct(
    protected SourceFile $file,
    protected ErrorReporter $reporter)
  {
    $this->offset = 0;
  }

  protected function location(): Location
  {
    return $this->file->location($this->offset);
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
    $formattedMessage = $this->location()->toString() . ": " . $message;
    $this->reporter->error($formattedMessage);
  }

  protected int $offset;
}
