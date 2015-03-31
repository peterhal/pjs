<?hh //strict

class IndentedWriter {
  public function __construct(
    private resource $file)
  {
    $this->atBeginingOfLine  = true;
    $this->indent = 0;
  }

  public function write(string $value): void
  {
    if ($this->atBeginingOfLine) {
      fwrite($this->file, str_repeat(' ', $this->indent));
    }
    fwrite($this->file, $value);
    $this->atBeginingOfLine = false;
  }

  public function writeLine(): void
  {
    fwrite($this->file, "\n");
    $this->atBeginingOfLine = true;
  }

  public function indent(): void
  {
    $this->indent += self::INDENT;
  }

  public function outdent(): void
  {
    $this->indent -= self::INDENT;

    if ($this->indent < 0) {
      throw new Exception();
    }
  }

  const int INDENT = 2;

  private bool $atBeginingOfLine;
  private int $indent;
}
