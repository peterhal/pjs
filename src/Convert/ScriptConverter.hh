<?hh // strict

namespace Convert {

  require_once 'Utils/IndentedWriter.hh';
  require_once 'Syntax/ParseTree.hh';
  require_once 'Syntax/ParseTreeKind.hh';
  require_once 'Syntax/ParseTrees.hh';
  require_once 'Syntax/Trees.hh';
  require_once 'DeclarationConverter.hh';
  require_once 'ClassConverter.hh';
  require_once 'EnumConverter.hh';

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
  use Syntax\InterfaceDeclarationTree;
  use Syntax\EnumDeclarationTree;

class ScriptConverter extends DeclarationConverter
{
  public function __construct(
    IndentedWriter $writer)
  {
    parent::__construct($writer);
  }

  public function convertScript(ScriptTree $tree): void
  {
    // Wrap the whole file in an IIFE
    $this->write('(function(' . self::$export . ') {');
    $this->writeLine();
    $this->indent();
    $this->convertDeclarations($tree->declarations);
    $this->outdent();
    $this->write('}(this));');
    $this->writeLine();
  }

  public function convertDeclarationsOpt(?Vector<ParseTree> $declarations): void
  {
    if ($declarations !== null) {
      $this->convertDeclarations($declarations);
    }
  }

  public function convertDeclarations(Vector<ParseTree> $declarations): void
  {
    foreach ($declarations as $declaration) {
      switch ($declaration->kind()) {
      case ParseTreeKind::REQUIRE_ONCE_DIRECTIVE:
        break;
      case ParseTreeKind::NAMESPACE_USE_DECLARATION:
        $this->convertNamespaceUseDeclaration($declaration->asNamespaceUseDeclaration());
        break;
      case ParseTreeKind::FUNCTION_DEFINITION:
        $this->convertFunctionDefinition($declaration->asFunctionDefinition());
        break;
      case ParseTreeKind::NAMESPACE_DEFINITION:
        $this->convertNamespaceDefinition($declaration->asNamespaceDefinition());
        break;
      case ParseTreeKind::CLASS_DECLARATION:
        $this->convertClassDeclaration($declaration->asClassDeclaration());
        break;
      case ParseTreeKind::ENUM_DECLARATION:
        $this->convertEnumDeclaration($declaration->asEnumDeclaration());
        break;
      case ParseTreeKind::INTERFACE_DECLARATION:
        $this->convertInterfaceDeclaration($declaration->asInterfaceDeclaration());
        break;
      default:
        throw $this->unknownTree($declaration);
      }
    }
  }

  public function convertEnumDeclaration(EnumDeclarationTree $tree): void
  {
    (new EnumConverter($this->writer, $tree))->convertEnumDeclaration();
  }

  public function convertInterfaceDeclaration(InterfaceDeclarationTree $tree): void
  {
    // Interfaces produce no code.
  }

  public function convertClassDeclaration(ClassDeclarationTree $tree): void
  {
    (new ClassConverter($this->writer, $tree))->convertClassDeclaration();
  }

  public function convertNamespaceDefinition(NamespaceDefinitionTree $tree): void
  {
    if ($tree->name !== null) {
      $name = $tree->name->text();
      $this->write('var ' . $name . ' = ' . $name . ' || {};'); 
      $this->writeLine();
      $this->write('(function(' . self::$export . ') {');
      $this->writeLine();

      $this->convertDeclarationsOpt($tree->declarations);

      $this->write('}(' . $name .'));');
      $this->writeLine();
    } else {
      throw new \Exception();
    }
  }

  public function convertFunctionDefinition(
    FunctionDefinitionTree $tree): void
  {
    $name = $tree->name->value();

    $this->write('function ' . $name);
    $this->convertParameterList($tree->parameters->asParameterList());

    $this->convertCompoundStatement($tree->body->asCompoundStatement());
    $this->write(';');
    $this->writeLine();

    $this->writeExport($name);
  }

  public function convertNamespaceUseDeclaration(
    NamespaceUseDeclarationTree $tree): void
  {
    foreach ($tree->useClauses as $tree) {
      $useClause = $tree->asNamespaceUseClause();
      $name = Trees::nameOfNamespaceUseClause(
        $useClause->asNamespaceUseClause());
      $this->write(
        'var ' 
        . $name 
        . ' = '
        . $this->qualifiedNameToString($useClause->name->asQualifiedName())
        . ';');
      $this->writeLine();
    }
  }
}

}

