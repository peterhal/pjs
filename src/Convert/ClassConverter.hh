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
  use Syntax\PredefinedName;
  use Syntax\Trees;
  use Syntax\Token;
  use Syntax\TokenKind;
  use Syntax\ClassDeclarationTree;
  use Syntax\ConstructorDeclarationTree;
  use Syntax\ConstDeclarationTree;
  use Syntax\ConstDeclaratorTree;
  use Syntax\MethodDefinitionTree;
  use Syntax\FunctionCallTree;
  use Syntax\PropertyDeclarationTree;

class ClassConverter extends DeclarationConverter
{
  public function __construct(
    IndentedWriter $writer,
    private ClassDeclarationTree $tree)
  {
    parent::__construct($writer);
    $this->name = $tree->name->text();
    $this->ctorTree = Trees::ctorOfClassDeclaration($tree);
  }

  public function convertClassDeclaration(): void
  {
    // var ctor = function ...
    $ctorTree = $this->ctorTree;
    if ($ctorTree === null) {
      $this->write('var ' . $this->name . ' = function() {};');
      $this->writeLine();
    } else {
      $this->write('var ' . $this->name . ' = function');
      $this->convertConstructorParameters($ctorTree->parameters);
      $this->write('{');
      $this->writeLine();
      $this->indent();

      $this->writeThisDeclaration();
      $this->convertConstructorParameterInitializers($ctorTree->parameters);
      $this->convertStatementList($ctorTree->body->asCompoundStatement()->statements);

      $this->outdent();
      $this->write('}');
      $this->writeLine();
      $this->writeLine();
    }

    $extendsClause = $this->tree->extendsClause;
    if ($extendsClause !== null) {
      // ctor.prototype = (function() { 
      //   function t(){}; 
      //   t.prototype = base; 
      //   return new t();
      // } ();
      $this->write($this->name . '.prototype = (function() { function __t(){}; __t.prototype = ');
      $this->convertBaseClass();
      $this->write('.prototype; return new __t(); } ());');
      $this->writeLine();
    }

    // TODO: traits

    foreach ($this->tree->members as $member) {
      $this->convertClassMember($member);
    }

    $this->writeExport($this->name);
  }

  public function writeThisDeclaration(): void
  {
    $this->write('var $this = this;');
    $this->writeLine();
  }

  public function convertSelf(): void
  {
    $this->write($this->name);
  }

  public function convertParent(): void
  {
    throw new \Exception('parent within a class');
  }

  public function convertBaseClass(): void
  {
    $extendsClause = $this->tree->extendsClause;
    if ($extendsClause === null) {
      throw new \Exception('use of parent in class without a base');
    }
    $this->convertQualifiedName($extendsClause->asNamedType()->name->asQualifiedName());
  }

  public function convertFunctionCall(FunctionCallTree $tree): void
  {
    if ($tree->function->isScopeResolution()
        && Trees::isParent($tree->function->asScopeResolution()->baseName)) {
      $memberName = $tree->function->asScopeResolution()->memberName->text();
      $this->convertBaseClass();
      if ($memberName !== PredefinedName::__construct) {
        $this->write('.prototype.' . $memberName);
      }
      $this->write('.call($this');
      if ($tree->arguments !== null) {
        foreach ($tree->arguments as $argument) {
          $this->write(', ');
          $this->convertExpression($argument);
        }
      }
      $this->write(')');
    } else {
      parent::convertFunctionCall($tree);
    }
  }

  private function convertConstructorParameterInitializers(?Vector<ParseTree> $parameters): void
  {
    if ($parameters !== null) {
      foreach ($parameters as $tree) {
        $parameter = $tree->asConstructorParameter();
        if ($parameter->modifiers !== null) {
          $name = $parameter->name->text();
          $this->write('$this.' . $name . ' = ' . $name . ';');
          $this->writeLine();
        }
      }
    }
  }

  private function convertConstructorParameters(?Vector<ParseTree> $parameters): void
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
        $this->write($parameter->asConstructorParameter()->name->text());
      }
    }
    $this->write(')');
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
    case ParseTreeKind::CONSTRUCTOR_DECLARATION:
      // ctors are handled with the class header.
      break;
    case ParseTreeKind::PROPERTY_DECLARATION:
      $this->convertPropertyDeclaration($tree->asPropertyDeclaration());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  private function convertPropertyDeclaration(PropertyDeclarationTree $tree): void
  {
    $isStatic = !$tree->modifiers
      ->filter($token ==> $token->kind() === TokenKind::KW_STATIC)->isEmpty();

    foreach ($tree->declarators as $element) {
      $declarator = $element->asPropertyDeclarator();
      $initializer = $declarator->initializer;
      if ($initializer !== null) {
        if ($isStatic) {
          $this->write($this->name . '.' . $declarator->name->text() . ' = ');
        } else {
          throw new \Exception('Instance properties with initializers.');
        }
        $this->convertExpression($initializer);
        $this->writeEndStatement();
      }
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
      $this->convertParameterList($tree->parameters->asParameterList());
      $this->writeLine();
      $this->write('{');
      $this->writeLine();
      $this->indent();

      $this->writeThisDeclaration();
      $this->convertCompoundStatement($body->asCompoundStatement());

      $this->outdent();
      $this->write('}');
      $this->writeLine();
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
  private ?ConstructorDeclarationTree $ctorTree;
}

}

