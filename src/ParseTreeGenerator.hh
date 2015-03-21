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

  public static function main(array<string> $argv): int
  {
    $kindFileName = $argv[1];

    ParseTreeGenerator::writeKindFile($kindFileName);

    return 0;
  }
}
