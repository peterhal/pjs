<?hh //strict

require_once 'Utils/ConsoleErrorReporter.hh';
require_once 'Utils/IndentedWriter.hh';
require_once 'Syntax/Parser.hh';
require_once 'Convert/ScriptConverter.hh';

use Utils\IndentedWriter;
use Utils\SourceFile;
use Utils\ConsoleErrorReporter;
use Syntax\Parser;
use Convert\ScriptConverter;

function convertMain(array<string> $argv) : int
{
  if (count($argv) < 2) {
    fwrite(STDERR, "Error: Missing required option 'filename'.\n");
    return 1;
  }
  for ($index = 1; $index < count($argv); $index++) {
    $filename = $argv[$index];

    $file = SourceFile::read($filename);
    $reporter = new ConsoleErrorReporter();
    $tree = Parser::parseFile($file, $reporter);

    $converter = new ScriptConverter(
      new IndentedWriter(STDOUT));
    $converter->convertScript($tree->asScript());
  }

  return 0;
}

