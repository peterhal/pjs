<?hh // strict

namespace Convert {

  require_once 'DeclarationConverter.hh';
  require_once 'Utils/IndentedWriter.hh';
  require_once 'Syntax/ParseTree.hh';
  require_once 'Syntax/ParseTreeKind.hh';
  require_once 'Syntax/ParseTrees.hh';
  require_once 'Syntax/Trees.hh';
  require_once 'StatementConverter.hh';

  use Exception;
  use Utils\IndentedWriter;
  use Syntax\ParseTree;
  use Syntax\ParseTreeKind;
  use Syntax\Trees;
  use Syntax\ClassDeclarationTree;
  use Syntax\ConstructorDeclarationTree;

class ClassConverter extends DeclarationConverter
{
  public function __construct(
    IndentedWriter $writer,
    private ClassDeclarationTree $tree)
  {
    parent::__construct($writer);
    $this->name = $tree->name->text();
    $this->className = self::$export . '.' . $this->name;
    $this->ctorTree = Trees::ctorOfClassDeclaration($tree);
  }

  public function convertClassDeclaration(): void
  {
    if ($this->tree->extendsClause !== null) {
      throw new Exception('TODO');
    }

    // __export.ctor = function ...
    if ($this->ctorTree === null) {
      $this->write($this->className . ' = function() {};');
      $this->writeLine();
    } else {
      $this->write($this->className . ' = function(');
      // TODO: parameters
      throw new Exception('TODO: ctor function params.');
      /*
      $this->write(') {');
      $this->writeLine();
      $this->indent();
      // TODO: body
      $this->outdent();
      $this->write('}');
      $this->writeLine();
       */
    }

    // TODO: members
  }

  private string $name;
  private string $className;
  private ?ConstructorDeclarationTree $ctorTree;
}

}

