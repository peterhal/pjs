<?hh //strict

require_once 'Char.hh';

class TypeSpec
{
  public function __construct(
      public string $name)
  {}
}

class ParseTreeSpecification
{
  public function __construct(
      public string $name,
      public Vector<Pair<string, string>> $fields)
  {}

  public static function CamelToUpper(string $value): string
  {
    $result = $value[0];

    for ($i = 1; $i < strlen($value); $i++) {
      if (Char::isUpperCaseLetter(ord($value[$i]))) {
        $result .= '_';
      }

      $result .= strtoupper($value[$i]);
    }

    return $result;
  }

  public function upperName(): string 
  {
    return ParseTreeSpecification::CamelToUpper($this->name);
  }
}

class ParseTreeSpecifications
{
  public static Vector<ParseTreeSpecification> $specs = Vector {};
}

class ParseTreeGenerator
{

  private static function writeKindFile(string $filename): void
  {
    ParseTreeSpecifications::$specs[] = 
      new ParseTreeSpecification(
          "Script",
          Vector {
            Pair { "declarations", "Vector<ParseTree>" }
          });

    $file = fopen($filename, 'w');

    fwrite($file, <<< FILE_HEADER
<<?hh // strict

enum ParseTreeKind : int
{

FILE_HEADER
);
    $index = 0;
    foreach (ParseTreeSpecifications::$specs as $spec) {
      $name = $spec->upperName();
      fwrite($file, "  $name = $index;\n");
      $index++;
    }
    fwrite($file, "}\n");
    fclose($file);
  }

  private static function writeParseTreeFile(string $filename): void
  {
    $file = fopen($filename, 'w');
    fwrite($file, <<< 'FILE_HEADER'
<?hh //strict

require_once 'ParseTreeKind.hh';

class ParseTree
{
  public function __construct(
    private Range $range,
    private ParseTreeKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): ParseTreeKind { return $this->kind; }

FILE_HEADER
);
    foreach (ParseTreeSpecifications::$specs as $spec) {
      $name = $spec->name;
      $upperName = $spec->upperName();
      $tree = $name . "Tree";
      fwrite($file, <<<IS
  public function is$name(): bool {
    return \$this->kind === ParseTreeKind::$upperName;
  }

IS
);
      fwrite($file, <<<AS
  public function as$name(): $tree { 
    invariant(\$this instanceof $tree, "Wrong type.");
    return \$this;
  }

AS
);
    }

    fwrite($file, "}\n");
    fclose($file);
  }

  public static function main(array<string> $argv): int
  {
    $kindFileName = $argv[1];
    $parseTreeFileName = $argv[2];
    ParseTreeGenerator::writeKindFile($kindFileName);
    ParseTreeGenerator::writeParseTreeFile($parseTreeFileName);
    return 0;
  }
}
