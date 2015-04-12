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

class StatementConverter extends ExpressionConverter
{
  public function __construct(
    IndentedWriter $writer)
  {
    parent::__construct($writer);
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
}

}

