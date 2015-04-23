<?hh // strict

namespace Convert {

  require_once 'DeclarationConverter.hh';
  require_once 'Syntax/ParseTrees.hh';
  require_once 'Utils/IndentedWriter.hh';

  use Utils\IndentedWriter;
  use Syntax\EnumDeclarationTree;
  use Syntax\EnumeratorTree;

class EnumConverter extends DeclarationConverter {
  public function __construct(
    IndentedWriter $writer,
    private EnumDeclarationTree $tree)
  {
    parent::__construct($writer);

    $this->name = $tree->name->text();
  }

  public function convertEnumDeclaration(): void
  {
    $this->write('var ' . $this->name . ' = {};');
    $this->writeLine();

    if ($this->tree->enumerators !== null) {
      foreach ($this->tree->enumerators as $enumerator) {
        $this->convertEnumerator($enumerator->asEnumerator());
      }
    }

    // getNames
    $this->write($this->name . '.getNames = (function() { var names = {};');
    $this->writeLine();
    $this->indent();
    if ($this->tree->enumerators !== null) {
      foreach ($this->tree->enumerators as $enumerator) {
        $enumeratorTree = $enumerator->asEnumerator();
        $this->write('names[' . $this->name . '.' . $enumeratorTree->name->text() . '] = \'');
        $this->write($enumeratorTree->name->text() . '\';');
        $this->writeLine();
      }
    }
    $this->outdent();
    $this->write('return function() { return names; };}());');
    $this->writeLine();

    $this->writeExport($this->name);
  }

  // TODO: getNames, static member.

  public function convertEnumerator(EnumeratorTree $tree): void
  {
    $this->write($this->name . '.' . $tree->name->text() . ' = ');
    $this->convertExpression($tree->value);
    $this->write(';');
    $this->writeLine();
  }

  private string $name;
}

}
