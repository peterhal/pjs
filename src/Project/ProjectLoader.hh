<?hh // strict

namespace Project {

require_once 'Utils/SourceFile.hh';
require_once 'Utils/ErrorReporter.hh';
require_once 'Syntax/Parser.hh';
require_once 'Syntax/ParseTree.hh';

use Generator;
use Utils\ErrorReporter;
use Utils\SourceFile;
use Syntax\Parser;
use Syntax\ParseTree;
use Syntax\ScriptTree;

class Loader {
  public function __construct(private ErrorReporter $reporter)
  {
    $this->files = Map {};
    $this->trees = Map {};
  }

  public function addFile(string $filename): SourceFile
  {
    if ($this->files->containsKey($filename)) {
      return $this->files[$filename];
    } else {
      $result = SourceFile::read($filename);
      $this->files[$filename] = $result;
      $this->trees[$filename] = Parser::parseFile($result, $this->reporter);
      // TODO: Add required files.
      return $result;
    }
  }

  public static function dependencies(ScriptTree $tree): Generator<int, string, void>
  {
    foreach ($tree->declarations as $declaration) {
      if ($declaration->isRequireOnceDirective()) {
        // TODO: Strip quotes
        yield $declaration->asRequireOnceDirective()->includeFilename
          ->asLiteral()->value->text();
      }
    }
  }

  private Map<string, SourceFile> $files;
  private Map<string, ParseTree> $trees;
}

}
