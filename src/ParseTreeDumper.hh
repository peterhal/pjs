<?hh //strict

namespace Syntax {

require_once 'IndentedWriter.hh';
require_once 'Token.hh';
require_once 'TokenKind.hh';
require_once 'ParseTree.hh';
require_once 'ParseTreeKind.hh';

use Utils\IndentedWriter;
use \ReflectionClass;
use \ReflectionProperty;
use \Exception;

function parseTreeKindToString(ParseTreeKind $kind): string
{
  return ParseTreeKind::getNames()[$kind];
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

}

