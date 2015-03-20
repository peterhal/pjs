<?hh //strict

enum ParseTreeKind : int
{
  SCRIPT = 0;

  // Declarations
  REQUIRE_MULTIPLE_DIRECTIVE = 1;
  REQUIRE_ONCE_DIRECTIVE = 2;

  FUNCTION_DEFINITION = 3;
  PARAMETER_LIST = 4;
  PARAMETER_DECLARATION = 5;

  // Expressions
  LITERAL = 100;

  // Statements
  COMPOUND_STATEMENT = 200;

  // Types
  THIS_TYPE = 300;
}
