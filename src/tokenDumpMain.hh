<?hh //strict

require_once 'ConsoleErrorReporter.hh';
require_once 'SourceFile.hh';
require_once 'Lexer.hh';

use Utils\SourceFile;
use Utils\ConsoleErrorReporter;
use Syntax\Lexer;
use Syntax\Token;
use Syntax\tokenKindToString;

function dumpString(string $value): void
{
  fwrite(STDOUT, $value);
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

function tokenDumpMain(array<string> $argv) : int
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
  dumpTokens($tokens);

  return 0;
}

