<?hh //strict

require_once 'ConsoleErrorReporter.hh';
require_once 'Lexer.hh';
require_once 'Parser.hh';
require_once 'SourceFile.hh';

function main(array<string> $argv) : int
{
  if (count($argv) < 2) {
    fwrite(STDERR, "Error: Missing required option 'filename'.\n");
    return 1;
  }
  $filename = $argv[1];

  $file = SourceFile::read($filename);
  fwrite(STDOUT, $file->contents());

  $tokens = Lexer::lexFile($file, new ConsoleErrorReporter());
  for ($i = 0; $i < $tokens->count(); $i++) {
    var_dump(TokenKind::getNames()[$tokens[$i]->kind()]);
  }

  return 0;
}

