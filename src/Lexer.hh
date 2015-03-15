<?hh //strict

require_once 'Characters.hh';
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
      return Characters::EOF;
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

class Lexer extends LexerBase
{
  public static function lexFile(
    SourceFile $file,
    ErrorReporter $reporter): Vector<Token>
  {
    return (new Lexer($file, $reporter))->lexAll();
  }

  public function __construct(
    SourceFile $file,
    ErrorReporter $reporter)
  {
    parent::__construct($file, $reporter);
  }

  public function lexAll(): Vector<Token>
  {
    $this->skipWhitespaceAndComments();
    return Vector{};
  }

  public function nextToken(): Token
  {
    $this->skipWhitespaceAndComments();
    return new Token($this->location(), TokenKind::EOF);
  }

  private function skipWhitespaceAndComments(): void
  {
    do {
      $this->skipWhitespace();
    } while ($this->skipComment());
  }

  private function skipWhitespace(): void
  {
    while (!$this->eof() && Characters::isWhitespace($this->peek())) {
      $this->next();
    }
  }

  private function skipComment(): bool
  {
    if ($this->peekChar(Characters::HASH)) {
      $this->next();
      $this->skipToEndOfLine();
      return true;
    } else if ($this->peekChar(Characters::FORWARD_SLASH)) {
      if ($this->peekCharOffset(Characters::FORWARD_SLASH, 1)) {
        $this->next();
        $this->next();
        $this->skipToEndOfLine();
        return true;
      } else if ($this->peekCharOffset(Characters::STAR, 1)) {
        $this->next();
        $this->next();
        $this->skipToEndOfDelimitedComment();
        return true;
      }
    }

    return false;
  }

  private function skipToEndOfLine(): void
  {
    while (!$this->eof() && !Characters::isNewLine($this->peek())) {
      $this->next();
    }
  }
  private function skipToEndOfDelimitedComment(): void
  {
    while (true) {
      if ($this->eof()) {
        $this->error("Expected end of delimited comment.");
        return;
      }
      if ($this->peekCharOffset(Characters::STAR, 0)
        && $this->peekCharOffset(Characters::FORWARD_SLASH, 1)) {
        $this->next();
        $this->next();
        return;
      }
      $this->next();
    }
  }
}

