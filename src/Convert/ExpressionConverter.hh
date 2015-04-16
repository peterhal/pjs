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
  use Syntax\Trees;
  use Syntax\QualifiedNameTree;
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
  use Syntax\UnaryExpressionTree;
  use Syntax\ParenExpressionTree;
  use Syntax\ConditionalExpressionTree;

class ExpressionConverter 
{
  public function __construct(
    protected IndentedWriter $writer)
  {
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
    case ParseTreeKind::UNARY_EXPRESSION:
      $this->convertUnaryExpression($tree->asUnaryExpression());
      break;
    case ParseTreeKind::PAREN_EXPRESSION:
      $this->convertParenExpression($tree->asParenExpression());
      break;
    case ParseTreeKind::CONDITIONAL_EXPRESSION:
      $this->convertConditionalExpression($tree->asConditionalExpression());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  public function convertConditionalExpression(ConditionalExpressionTree $tree): void
  {
    $this->convertExpression($tree->condition);
    $this->write(' ? ');
    if ($tree->trueVaule === null) {
      throw new Exception('conditional without first value.');
    } else {
      $this->convertExpression($tree->trueVaule);
    }
    $this->write(' : ');
    $this->convertExpression($tree->falseValue);
  }

  public function convertUnaryExpression(UnaryExpressionTree $tree): void
  {
    $this->convertPrefixOperatorToken($tree->operator);
    $this->convertExpression($tree->value);
  }

  public function convertPrefixOperatorToken(Token $token): void
  {
    $this->write($this->prefixTokenKindToString($token->kind()));
  }

  public function prefixTokenKindToString(TokenKind $operator): string
  {
    switch ($operator) {
    case TokenKind::MINUS: return '-';
    case TokenKind::PLUS_PLUS: return '++';
    default:
      throw $this->unknownTokenKind($operator);
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

  public function convertParenExpression(ParenExpressionTree $tree): void
  {
    $this->convertExpressionWithParens($tree->expression);
  }

  public function convertExpressionWithParens(ParseTree $tree): void
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
    case TokenKind::CLOSE_ANGLE: return '>';
    case TokenKind::LESS_EQUAL: return '<=';
    case TokenKind::GREATER_EQUAL: return '>=';
    case TokenKind::EQUAL: return '=';
    case TokenKind::EQUAL_EQUAL_EQUAL: return '===';
    case TokenKind::BANG_EQUAL_EQUAL: return '!==';
    case TokenKind::BAR_BAR: return '||';
    case TokenKind::AMPERSAND_AMPERSAND: return '&&';
    case TokenKind::PERIOD: return '+';
    case TokenKind::PLUS: return '+';
    case TokenKind::MINUS: return '-';
    case TokenKind::PLUS_EQUAL: return '+=';
    case TokenKind::MINUS_EQUAL: return '-=';
    default:
      throw $this->unknownTokenKind($operator);
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

  protected function write(string $value): void
  {
    $this->writer->write($value);
  }

  protected function writeLine(): void
  {
    $this->writer->writeLine();
  }

  protected function indent(): void
  {
    $this->writer->indent();
  }

  protected function outdent(): void
  {
    $this->writer->outdent();
  }

  protected function unknownTokenKind(TokenKind $kind): Exception
  {
    return new Exception('Unexpected ' . \Syntax\tokenKindToString($kind));
  }

  protected function unknownToken(Token $token): Exception
  {
    return $this->unknownTokenKind($token->kind());
  }

  protected function unknownTree(ParseTree $tree): Exception
  {
    return new Exception('Unexpected ' . \Syntax\parseTreeKindToString($tree->kind()));
  }
}

}
