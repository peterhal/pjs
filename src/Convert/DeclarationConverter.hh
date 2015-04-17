<?hh // strict

namespace Convert {

  require_once 'StatementConverter.hh';
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
  use Syntax\ParameterListTree;

class DeclarationConverter extends StatementConverter
{
  public function __construct(
    IndentedWriter $writer)
  {
    parent::__construct($writer);
  }

  protected static string $export = '__export';

  protected function convertParameterList(ParameterListTree $parameters): void
  {
    $this->convertParameters($parameters->parameters);
  }

  protected function convertParameters(?Vector<ParseTree> $parameters): void
  {
    $this->write('(');
    if ($parameters !== null) {
      $first = true;
      foreach ($parameters as $parameter) {
        if ($first) {
          $first = false;
        } else {
          $this->write(', ');
        }
        $this->write($parameter->asParameterDeclaration()->name->value());
      }
    }
    $this->write(')');
  }

  protected function writeExport(string $name): void
  {
    $this->write(DeclarationConverter::$export . '.' . $name . ' = ' . $name . ';');
    $this->writeLine();
  }
}

}

