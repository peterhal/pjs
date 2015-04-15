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
  use Syntax\Token;
  use Syntax\TokenKind;
  use Syntax\ClassDeclarationTree;
  use Syntax\ConstructorDeclarationTree;
  use Syntax\ConstDeclarationTree;
  use Syntax\ConstDeclaratorTree;
  use Syntax\MethodDefinitionTree;

class ClassConverter extends DeclarationConverter
{
  public function __construct(
    IndentedWriter $writer,
    private ClassDeclarationTree $tree)
  {
    parent::__construct($writer);
    $this->name = $tree->name->text();
    $this->exportName = self::$export . '.' . $this->name;
    $this->ctorTree = Trees::ctorOfClassDeclaration($tree);
  }

  public function convertClassDeclaration(): void
  {
    if ($this->tree->extendsClause !== null) {
      throw new Exception('TODO');
    }

    // var ctor = function ...
    if ($this->ctorTree === null) {
      $this->write('var ' . $this->name . ' = function() {};');
      $this->writeLine();
    } else {
      $this->write('var ' . $this->name . ' = function(');
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

    // TODO: traits

    foreach ($this->tree->members as $member) {
      $this->convertClassMember($member);
    }

    // __export.ctor = ctor;
    $this->write($this->exportName . ' = ' . $this->name . ';');
    $this->writeLine();
  }

  private function convertClassMember(ParseTree $tree) : void
  {
    switch ($tree->kind()) {
    case ParseTreeKind::CONST_DECLARATION:
      $this->convertConstDeclaration($tree->asConstDeclaration());
      break;
    case ParseTreeKind::METHOD_DEFINITION:
      $this->convertMethodDefinition($tree->asMethodDefinition());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  private function convertMethodDefinition(MethodDefinitionTree $tree): void
  {
    $isStatic = !$tree->modifiers
      ->filter($token ==> $token->kind() === TokenKind::KW_STATIC)->isEmpty();
    $name = $tree->name->text();
    $body = $tree->body;
    if ($body !== null) {
      if ($isStatic) {
        $this->write($this->name . '.' . $name . ' = function ');
      } else {
        $this->write($this->name . '.prototype.' . $name . ' = function ');
      }
      $this->convertParameters($tree->parameters->asParameterList());
      $this->convertCompoundStatement($body->asCompoundStatement());
      $this->writeLine();
    }
  }

  private function convertConstDeclaration(ConstDeclarationTree $tree): void
  {
    foreach ($tree->declarators as $declarator) {
      $this->convertConstDeclarator($declarator->asConstDeclarator());
    }
  }

  private function convertConstDeclarator(ConstDeclaratorTree $tree): void
  {
      $this->write($this->name . '.' . $tree->name->text() . ' = ');
      $this->convertExpression($tree->value);
      $this->write(';');
      $this->writeLine();
  }

  private string $name;
  private string $exportName;
  private ?ConstructorDeclarationTree $ctorTree;
}

}

