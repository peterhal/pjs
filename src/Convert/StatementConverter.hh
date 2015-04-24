<?hh // strict

namespace Convert {

  require_once 'ExpressionConverter.hh';
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
  use Syntax\CompoundStatementTree;
  use Syntax\ExpressionStatementTree;
  use Syntax\IfStatementTree;
  use Syntax\ElseifClauseTree;
  use Syntax\ElseClauseTree;
  use Syntax\ReturnStatementTree;
  use Syntax\ForStatementTree;
  use Syntax\ForEachStatementTree;
  use Syntax\WhileStatementTree;
  use Syntax\DoStatementTree;
  use Syntax\SwitchStatementTree;
  use Syntax\ThrowStatementTree;
  use Syntax\BreakStatementTree;
  use Syntax\ContinueStatementTree;

abstract class StatementConverter extends ExpressionConverter
{
  public function __construct(
    IndentedWriter $writer)
  {
    parent::__construct($writer);
    $this->foreachNesting = 0;
  }

  private int $foreachNesting;

  public function convertCompoundStatement(
    CompoundStatementTree $tree): void
  {
    $this->write('{');
    $this->writeLine();
    $this->indent();

    $this->convertStatementList($tree->statements);

    $this->outdent();
    $this->write('}');
    $this->writeLine();
  }

  public function convertStatementList(Vector<ParseTree> $statements): void
  {
    foreach ($statements as $statement) {
      $this->convertStatement($statement);
    }
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
    case ParseTreeKind::FOR_EACH_STATEMENT:
      $this->convertForEachStatement($tree->asForEachStatement());
      break;
    case ParseTreeKind::WHILE_STATEMENT:
      $this->convertWhileStatement($tree->asWhileStatement());
      break;
    case ParseTreeKind::DO_STATEMENT:
      $this->convertDoStatement($tree->asDoStatement());
      break;
    case ParseTreeKind::SWITCH_STATEMENT:
      $this->convertSwitchStatement($tree->asSwitchStatement());
      break;
    case ParseTreeKind::THROW_STATEMENT:
      $this->convertThrowStatement($tree->asThrowStatement());
      break;
    case ParseTreeKind::BREAK_STATEMENT:
      $this->convertBreakStatement($tree->asBreakStatement());
      break;
    case ParseTreeKind::CONTINUE_STATEMENT:
      $this->convertContinueStatement($tree->asContinueStatement());
      break;
    default:
      throw $this->unknownTree($tree);
    }
  }

  public function convertForEachStatement(ForEachStatementTree $tree): void
  {
    $collection = '__collection' . $this->foreachNesting;
    $index = '__index' . $this->foreachNesting;
    $this->foreachNesting++;
    $this->write('for (var ' . $collection . ' = ');
    $this->convertExpression($tree->collection);
    $this->write(', ' . $index . ' = 0; ' . $index . ' < ' . $collection . '.length; ' . $index . '++) {');
    $this->writeLine();
    if ($tree->key !== null) {
      throw new \Exception('keyed foreach');
    }
    $this->write('var ' . $tree->value->asVariableName()->name->text() . ' = ' . $collection . '[' . $index . '];');
    $this->writeLine();
    $this->convertIndentedStatement($tree->body);
    $this->write('}');
    $this->writeLine();
    $this->foreachNesting--;
  }

  public function convertBreakStatement(BreakStatementTree $tree): void
  {
    $this->write('break');
    $this->writeEndStatement();
  }

  public function convertContinueStatement(ContinueStatementTree $tree): void
  {
    $this->write('continue');
    $this->writeEndStatement();
  }

  public function convertDoStatement(DoStatementTree $tree): void
  {
    $this->write('do ');
    $this->convertStatement($tree->body);
    $this->write(' while ');
    $this->convertExpressionWithParens($tree->condition);
    $this->writeLine();
  }

  public function convertWhileStatement(WhileStatementTree $tree): void
  {
    $this->write('while ');
    $this->convertExpressionWithParens($tree->condition);
    $this->writeLine();
    $this->convertIndentedStatement($tree->body);
  }

  public function convertThrowStatement(ThrowStatementTree $tree): void
  {
    $this->write('throw ');
    $this->convertExpression($tree->value);
    $this->writeEndStatement();
  }

  public function convertSwitchStatement(SwitchStatementTree $tree): void
  {
    $this->write('switch ');
    $this->convertExpressionWithParens($tree->condition);
    $this->write('{');
    $this->writeLine();
    foreach ($tree->caseClauses as $caseClause) {
      foreach ($caseClause->asCaseClause()->labels as $label) {
        if ($label->isCaseLabel()) {
          $this->write('case ');
          $this->convertExpression($label->asCaseLabel()->condition);
          $this->write(':');
          $this->writeLine();
        } else {
          $this->write('default:');
          $this->writeLine();
        }
      }
      $this->indent();
      $this->convertStatementList($caseClause->asCaseClause()->statements);
      $this->outdent();
    }
    $this->write('}');
    $this->writeLine();
  }

  public function convertForStatement(ForStatementTree $tree): void
  {
    $this->write('for (');
    if ($tree->initializer !== null && self::isLocalVarAssignment($tree->initializer[0])) {
      $this->write('var ');
    }
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

  public function convertIndentedStatement(ParseTree $tree): void
  {
    $this->indent();
    $this->convertStatement($tree);
    $this->outdent();
  }

  public static function isLocalVarAssignment(ParseTree $tree): bool
  {
    return $tree->isBinaryExpression()
        && $tree->asBinaryExpression()->operator->kind() == TokenKind::EQUAL
        && $tree->asBinaryExpression()->left->isVariableName();
  }

  public function convertExpressionStatement(ExpressionStatementTree $tree): void
  {
    $expression= $tree->expression;
    if (self::isLocalVarAssignment($expression)) {
      $this->write('var ');
    }
    $this->convertExpression($expression);
    $this->writeEndStatement();
  }

  public function convertIfStatement(IfStatementTree $tree): void
  {
    $this->write('if ');
    $this->convertExpressionWithParens($tree->condition);
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
    $this->convertExpressionWithParens($tree->condition);
    $this->writeLine();
    $this->convertIndentedStatement($tree->elseClause);
  }

  public function convertElseClause(ElseClauseTree $tree): void
  {
    $this->write('else');
    $this->writeLine();
    $this->convertIndentedStatement($tree->elseClause);
  }
}

}

