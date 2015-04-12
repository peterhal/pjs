<?hh // strict

namespace Convert {

  require_once 'Utils/IndentedWriter.hh';
  require_once 'Syntax/ParseTree.hh';
  require_once 'Syntax/ParseTreeKind.hh';
  require_once 'Syntax/ParseTrees.hh';
  require_once 'Syntax/Trees.hh';

  use Exception;
  use Utils\IndentedWriter;
  use Syntax\Token;
  use Syntax\TokenKind;
  use Syntax\ParseTree;
  use Syntax\ParseTreeKind;
  use Syntax\ScriptTree;
  use Syntax\Trees;
  use Syntax\NamespaceUseDeclarationTree;
  use Syntax\NamespaceUseClauseTree;
  use Syntax\QualifiedNameTree;
  use Syntax\FunctionDefinitionTree;
  use Syntax\CompoundStatementTree;
  use Syntax\BinaryExpressionTree;
  use Syntax\IfStatementTree;
  use Syntax\FunctionCallTree;
  use Syntax\VariableNameTree ;
  use Syntax\LiteralTree;
  use Syntax\ExpressionStatementTree;
  use Syntax\SubscriptOperatorTree;
  use Syntax\ScopeResolutionTree;
  use Syntax\ObjectCreationExpressionTree;
  use Syntax\MemberSelectionTree;
  use Syntax\ElseifClauseTree;
  use Syntax\ElseClauseTree;
  use Syntax\ReturnStatementTree;
  use Syntax\ForStatementTree;
  use Syntax\PostfixOperatorTree;

class ScriptConverter 
{
  public function __construct(
    private IndentedWriter $writer)
  {
  }

  public function convertScript(ScriptTree $tree): void
  {
    // Wrap the whole file in an IIFE
    $this->write('(function() {');
    $this->writeLine();
    $this->indent();
    $this->convertDeclarations($tree->declarations);
    $this->outdent();
    $this->write('}())');
    $this->writeLine();
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
      case ParseTreeKind::CLASS_DECLARATION:
      case ParseTreeKind::ENUM_DECLARATION:
      case ParseTreeKind::INTERFACE_DECLARATION:
      default:
        throw $this->unknownTree($declaration);
      }
    }
  }

  public function convertFunctionDefinition(
    FunctionDefinitionTree $tree): void
  {
    $this->write('function ' . $tree->name->value() . '(');
    $parameters = $tree->parameters->asParameterList()->parameters;
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
    $this->write(') ');

    $this->convertCompoundStatement($tree->body->asCompoundStatement());
  }

  public function convertCompoundStatement(
    CompoundStatementTree $tree): void
  {
    $this->write('{');
    $this->writeLine();
    $this->indent();

    foreach ($tree->statements as $statement) {
      $this->convertStatement($statement);
    }

    $this->outdent();
    $this->write('}');
    $this->writeLine();
  }

  public function convertStatement(ParseTree $tree): void
  {
    switch ($tree->kind()) {
    case ParseTreeKind::IF_STATEMENT:
      $this->convertIfStatement($tree->asIfStatement());
      break;
    case ParseTreeKind::EXPRESSION_STATEMENT:
      $this->convertExpressionStatement($tree->asExpressionStatement());
      break;
    case ParseTreeKind::COMPOUND_STATEMENT:
      $this->convertCompoundStatement($tree->asCompoundStatement());
      break;
    case ParseTreeKind::RETURN_STATEMENT:
      $this->convertReturnStatement($tree->asReturnStatement());
      break;
    case ParseTreeKind::FOR_STATEMENT:
      $this->convertForStatement($tree->asForStatement());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  public function convertForStatement(ForStatementTree $tree): void
  {
    $this->write('for (');
    $this->convertExpressionListOpt($tree->initializer);
    $this->write('; ');
    $this->convertExpressionListOpt($tree->condition);
    $this->write('; ');
    if ($tree->increment !== null) {
      $this->convertExpressionListOpt($tree->increment);
    }
    $this->write(')');
    $this->writeLine();
    $this->convertIndentedStatement($tree->body);
  }

  public function writeEndStatement(): void
  {
    $this->write(';');
    $this->writeLine();
  }
  
  public function convertReturnStatement(ReturnStatementTree $tree): void
  {
    $this->write('return');
    $value = $tree->value;
    if ($value !== null) {
      $this->write(' ');
      $this->convertExpression($value);
    }
    $this->writeEndStatement();
  }

  public function convertCompoundStatementTree(CompoundStatementTree $tree): void
  {
    $this->write('{');
    $this->writeLine();
    $this->indent();
    foreach ($tree->statements as $statement) {
      $this->convertStatement($statement);
    }
    $this->outdent();
    $this->write('}');
    $this->writeLine();
  }

  public function convertIndentedStatement(ParseTree $tree): void
  {
    $this->indent();
    $this->convertStatement($tree);
    $this->outdent();
  }

  public function convertExpressionStatement(ExpressionStatementTree $tree): void
  {
    $this->convertExpression($tree->expression);
    $this->writeEndStatement();
  }

  public function convertIfStatement(IfStatementTree $tree): void
  {
    $this->write('if ');
    $this->convertParenExpression($tree->condition);
    $this->writeLine();
    $this->convertIndentedStatement($tree->thenClause);
    if ($tree->elseifClauses !== null) {
      foreach ($tree->elseifClauses as $elseifClause) {
        $this->convertElseIfClause($elseifClause->asElseifClause());
      }
    }
    if ($tree->elseClause !== null) {
      $this->convertElseClause($tree->elseClause->asElseClause());
    }
  }

  public function convertElseIfClause(ElseifClauseTree $tree): void
  {
    $this->write('else if ');
    $this->convertParenExpression($tree->condition);
    $this->writeLine();
    $this->convertIndentedStatement($tree->elseClause);
  }

  public function convertElseClause(ElseClauseTree $tree): void
  {
    $this->write('else');
    $this->writeLine();
    $this->convertIndentedStatement($tree->elseClause);
  }

  public function convertExpression(ParseTree $tree): void
  {
    switch ($tree->kind()) {
    case ParseTreeKind::BINARY_EXPRESSION:
      $this->convertBinaryExpression($tree->asBinaryExpression());
      break;
    case ParseTreeKind::FUNCTION_CALL:
      $this->convertFunctionCall($tree->asFunctionCall());
      break;
    case ParseTreeKind::QUALIFIED_NAME:
      $this->convertQualifiedName($tree->asQualifiedName());
      break;
    case ParseTreeKind::VARIABLE_NAME:
      $this->convertVariableName($tree->asVariableName());
      break;
    case ParseTreeKind::LITERAL:
      $this->convertLiteral($tree->asLiteral());
      break;
    case ParseTreeKind::SUBSCRIPT_OPERATOR:
      $this->convertSubscriptOperator($tree->asSubscriptOperator());
      break;
    case ParseTreeKind::SCOPE_RESOLUTION:
      $this->convertScopeResolution($tree->asScopeResolution());
      break;
    case ParseTreeKind::OBJECT_CREATION_EXPRESSION:
      $this->convertObjectCreationExpression($tree->asObjectCreationExpression());
      break;
    case ParseTreeKind::MEMBER_SELECTION:
      $this->convertMemberSelection($tree->asMemberSelection());
      break;
    case ParseTreeKind::POSTFIX_OPERATOR:
      $this->convertPostfixOperator($tree->asPostfixOperator());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  public function convertPostfixOperator(PostfixOperatorTree $tree): void
  {
    $this->convertExpression($tree->value);
    $this->convertPostfixOperatorToken($tree->operator);
  }

  public function convertPostfixOperatorToken(Token $token): void
  {
    $this->write($this->postfixTokenKindToString($token->kind()));
  }

  public function postfixTokenKindToString(TokenKind $operator): string
  {
    switch ($operator) {
    case TokenKind::PLUS_PLUS: return '++';
    default:
      throw $this->unknownTokenKind($operator);
    }
  }

  public function convertExpressionListOpt(?Vector<ParseTree> $expressions): void
  {
    if ($expressions !== null) {
      $first = true;
      foreach ($expressions as $expression) {
        if ($first) {
          $first = false;
        } else {
          $this->write(', ');
        }
        $this->convertExpression($expression);
      }
    }
  }

  public function convertParenExpression(ParseTree $tree): void
  {
    $this->write('(');
    $this->convertExpression($tree);
    $this->write(')');
  }

  public function convertMemberSelection(MemberSelectionTree $tree): void
  {
    $this->convertExpression($tree->object);
    // TODO: Handle the $ craziness
    $this->write('.' . $tree->name->text());
  }

  public function convertObjectCreationExpression(ObjectCreationExpressionTree $tree): void
  {
    $type = $tree->type;
    if ($type === null) {
      throw new Exception('static');
    } else {
      $this->write('new ');
      $this->convertType($type);
      $this->convertArgumentListOpt($tree->arguments);
    }
  }

  public function convertType(ParseTree $tree): void
  {
    switch ($tree->kind()) {
    case ParseTreeKind::QUALIFIED_NAME:
      $this->convertQualifiedName($tree->asQualifiedName());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  public function convertScopeResolution(ScopeResolutionTree $tree): void
  {
    $this->convertExpression($tree->baseName);
    $this->write('.');
    $this->write($tree->memberName->text());
  }

  public function convertSubscriptOperator(SubscriptOperatorTree $tree): void
  {
    $this->convertExpression($tree->collection);
    $index = $tree->index;
    if ($index !== null) {
      $this->write('[');
      $this->convertExpression($index);
      $this->write(']');
    } else {
      throw new \Exception();
    }
  }

  public function convertLiteral(LiteralTree $tree): void
  {
    $this->write($tree->value->text());
  }

  public function convertVariableName(VariableNameTree $tree): void
  {
    $this->write($tree->name->value());
  }

  public function convertFunctionCall(FunctionCallTree $tree): void
  {
    $this->convertExpression($tree->function);
    $this->convertArgumentListOpt($tree->arguments);
  }

  public function convertArgumentListOpt(?Vector<ParseTree> $arguments): void
  {
    $this->write('(');
    $this->convertExpressionListOpt($arguments);
    $this->write(')');
  }

  public function convertBinaryExpression(BinaryExpressionTree $tree): void
  {
    $this->convertExpression($tree->left);
    $this->convertBinaryOperator($tree->operator);
    $this->convertExpression($tree->right);
  }

  public function convertBinaryOperator(Token $operator): void
  {
    $this->write(' ' . $this->binaryOperatorToString($operator->kind()) . ' ');
  }

  public function binaryOperatorToString(TokenKind $operator): string
  {
    switch ($operator) {
    case TokenKind::OPEN_ANGLE: return '<';
    case TokenKind::EQUAL: return '=';
    case TokenKind::PERIOD: return '+';
    default:
      throw $this->unknownTokenKind($operator);
    }
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

  public function convertQualifiedName(QualifiedNameTree $tree): void
  {
    $this->write($this->qualifiedNameToString($tree));
  }

  public function qualifiedNameToString(QualifiedNameTree $tree): string
  {
    $result = '';
    $first = true;
    foreach ($tree->names as $name) {
      if ($first) {
        $first = false;
      } else {
        $result .= '.';
      }
      $result .= $name->value();
    }
    return $result;
  }

  private function write(string $value): void
  {
    $this->writer->write($value);
  }

  private function writeLine(): void
  {
    $this->writer->writeLine();
  }

  private function indent(): void
  {
    $this->writer->indent();
  }

  private function outdent(): void
  {
    $this->writer->outdent();
  }

  private function unknownTokenKind(TokenKind $kind): Exception
  {
    return new Exception('Unexpected ' . \Syntax\tokenKindToString($kind));
  }

  private function unknownToken(Token $token): Exception
  {
    return $this->unknownTokenKind($token->kind());
  }

  private function unknownTree(ParseTree $tree): Exception
  {
    return new Exception('Unexpected ' . \Syntax\parseTreeKindToString($tree->kind()));
  }
}

}

