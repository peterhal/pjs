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
  use Syntax\ScriptTree;
  use Syntax\Trees;
  use Syntax\NamespaceUseDeclarationTree;
  use Syntax\NamespaceUseClauseTree;
  use Syntax\FunctionDefinitionTree;
  use Syntax\NamespaceDefinitionTree;
  use Syntax\ClassDeclarationTree;

class ClassConverter extends DeclarationConverter
{
  public function __construct(
    IndentedWriter $writer)
  {
    parent::__construct($writer);
  }

  public function convertClassDeclaration(ClassDeclarationTree $tree): void
  {
    $name = $tree->name->text();
    $className = self::$export . '.' . $name;

    if ($tree->extendsClause !== null) {
      throw new Exception('TODO');
    }

    // __export.ctor = function ...
    $ctorTree = Trees::ctorOfClassDeclaration($tree);
    if ($ctorTree === null) {
      $this->write($className . ' = function() {};');
      $this->writeLine();
    } else {
      $this->write($className . ' = function(');
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
}

}

