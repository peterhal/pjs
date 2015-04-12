<?hh //strict

require_once 'Utils/ConsoleErrorReporter.hh';
require_once 'Utils/SourceFile.hh';
require_once 'Syntax/Parser.hh';

use Utils\SourceFile;
use Utils\ConsoleErrorReporter;
use Syntax\Parser;

function parseMain(array<string> $argv) : int
{
  if (count($argv) < 2) {
    fwrite(STDERR, "Error: Missing required option 'filename'.\n");
    return 1;
  }
  for ($index = 1; $index < count($argv); $index++) {
    $filename = $argv[$index];
    fwrite(STDOUT, $filename . "\n");

    Parser::parseFile(
      SourceFile::read($filename), 
      new ConsoleErrorReporter());
  }

  return 0;
}

