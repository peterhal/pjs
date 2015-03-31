<?hh //strict

require_once 'ConsoleErrorReporter.hh';
require_once 'Lexer.hh';
require_once 'Parser.hh';
require_once 'TokenKind.hh';
require_once 'SourceFile.hh';


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

function tokenKindToString(TokenKind $kind): string
{
  return TokenKind::getNames()[$kind];
}

function parseTreeKindToString(ParseTreeKind $kind): string
{
  return ParseTreeKind::getNames()[$kind];
}

function dumpString(string $value): void
{
  fwrite(STDOUT, $value);
}

function dumpFile(SourceFile $file): void
{
  dumpString($file->contents());
}

function dumpTokens(Vector<Token> $tokens): void
{
  for ($i = 0; $i < $tokens->count(); $i++) {
    dumpToken($tokens[$i]);
  }
}

function dumpToken(Token $token): void
{
    dumpString(tokenKindToString($token->kind()) . "\n");
}

class ParseTreeDumper
{
  public function __construct(resource $file)
  {
    $this->writer = new IndentedWriter($file);
  }

  public function dumpTree(ParseTree $tree): void
  {
    $this->writeLine();
    $this->dumpString("kind: " . parseTreeKindToString($tree->kind()));
    $properties = (new ReflectionClass($tree))
      ->getProperties(ReflectionProperty::IS_PUBLIC);
    foreach ($properties as $property) {
      $this->writeLine();
      $this->dumpString($property->getName() . ": ");
      $this->dumpValue($property->getValue($tree));
    }
  }

  public function dumpValue(mixed $value): void
  {
    if ($value === null) {
      $this->dumpString("null");
    } else if ($value === true) {
      $this->dumpString("true");
    } else if ($value === false) {
      $this->dumpString("false");
    } else if ($value instanceof ParseTree) {
      $this->indent();
      $this->dumpTree($value);
      $this->outdent();
    } else if ($value instanceof Vector) {
      if ($value->count() === 0) {
        $this->dumpString("<empty>");
      } else {
        $this->indent();
        $this->dumpVector($value);
        $this->outdent();
      }
    } else if ($value instanceof Token) {
      $this->dumpToken($value);
    } else {
      var_dump($value);
      throw new Exception("Unknown value!");
    }
  }

  public function dumpToken(Token $token): void
  {
    if ($token->isName() || $token->isVariableName() || $token->isLiteral()) {
      $this->dumpString(" " . $token->text());
    } else {
      $this->dumpString(tokenKindToString($token->kind()));
    }
  }

  public function dumpVector(Vector<mixed> $values): void
  {
    $index = 0;
    foreach ($values as $value) {
      $this->writeLine();
      $this->dumpString($index . ": ");
      $this->dumpValue($value);
      $index += 1;
    }
  }

  public function dumpString(string $value): void
  {
    $this->writer->write($value);
  }

  private function indent(): void
  {
    $this->writer->indent();
  }

  private function outdent(): void
  {
    $this->writer->outdent();
  }

  private function writeLine(): void
  {
    $this->writer->writeLine();
  }

  private IndentedWriter $writer;
}

function main(array<string> $argv) : int
{
  if (count($argv) < 2) {
    fwrite(STDERR, "Error: Missing required option 'filename'.\n");
    return 1;
  }
  $filename = $argv[1];
  fwrite(STDOUT, $filename . "\n");

  $file = SourceFile::read($filename);
  // dumpFile($file);

  $reporter = new ConsoleErrorReporter();
  $tokens = Lexer::lexFile($file, $reporter);
  // dumpTokens($tokens);

  $tree = Parser::parse($tokens, $reporter);
  $dumper = new ParseTreeDumper(STDOUT);
  $dumper->dumpTree($tree);
  fwrite(STDOUT, "\n");

  return 0;
}

