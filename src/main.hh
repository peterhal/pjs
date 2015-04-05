<?hh //strict

require_once 'ConsoleErrorReporter.hh';
require_once 'SourceFile.hh';
require_once 'Lexer.hh';
require_once 'Parser.hh';
require_once 'ParseTreeDumper.hh';

use Utils\SourceFile;
use Utils\ConsoleErrorReporter;
use Syntax\Lexer;
use Syntax\Parser;
use Syntax\ParseTreeDumper;

function main(array<string> $argv) : int
{
  if (count($argv) < 2) {
    fwrite(STDERR, "Error: Missing required option 'filename'.\n");
    return 1;
  }
  $filename = $argv[1];
  fwrite(STDOUT, $filename . "\n");

  $file = SourceFile::read($filename);

  $reporter = new ConsoleErrorReporter();
  $tokens = Lexer::lexFile($file, $reporter);

  $tree = Parser::parse($tokens, $reporter);
  $dumper = new ParseTreeDumper(STDOUT);
  $dumper->dumpTree($tree);
  fwrite(STDOUT, "\n");

  return 0;
}

