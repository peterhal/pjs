<?hh //strict

require_once 'ConsoleErrorReporter.hh';
require_once 'Lexer.hh';
require_once 'Parser.hh';
require_once 'TokenKind.hh';
require_once 'SourceFile.hh';

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

function dumpValue(mixed $value): void
{
  if ($value === null) {
    dumpString("null\n");
  } else if ($value === true) {
    dumpString("true\n");
  } else if ($value === false) {
    dumpString("false\n");
  } else if ($value instanceof ParseTree) {
    dumpTree($value);
  } else if ($value instanceof Vector) {
    dumpVector($value);
  } else if ($value instanceof Token) {
    dumpToken($value);
  } else {
    var_dump($value);
    throw new Exception("Unknown value!");
  }
}

function dumpVector(mixed $values): void
{
  $values2 = $values;
  if ($values instanceof Vector) {
    if ($values->count() === 0) {
      return;
    } else if ($values[0] instanceof ParseTree) {
      dumpVectorTree($values);
    } else if ($values2 instanceof Vector && $values2[0] instanceof Token) {
      dumpTokens($values2);
    } else {
      var_dump($values[0]);
      throw new Exception ("Unknown vector!");
    }
  } else {
    throw new Exception("Expecting Vector");
  }
}

function dumpVectorTree(Vector<ParseTree> $trees): void
{
  foreach ($trees as $tree) {
    dumpTree($tree);
  }
}

function dumpTree(ParseTree $tree): void
{
  dumpString(parseTreeKindToString($tree->kind()) . "\n");
  $properties = (new ReflectionClass($tree))
    ->getProperties(ReflectionProperty::IS_PUBLIC);
  foreach ($properties as $property) {
    dumpString($property->getName() . ": ");
    dumpValue($property->getValue($tree));
  }
}

function main(array<string> $argv) : int
{
  if (count($argv) < 2) {
    fwrite(STDERR, "Error: Missing required option 'filename'.\n");
    return 1;
  }
  $filename = $argv[1];

  $file = SourceFile::read($filename);
  // dumpFile($file);

  $reporter = new ConsoleErrorReporter();
  $tokens = Lexer::lexFile($file, $reporter);
  // dumpTokens($tokens);

  $tree = Parser::parse($tokens, $reporter);
  dumpTree($tree);

  return 0;
}

