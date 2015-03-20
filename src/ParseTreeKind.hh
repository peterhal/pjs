<?hh //strict

enum ParseTreeKind : int
{
  SCRIPT = 0;

  REQUIRE_MULTIPLE_DIRECTIVE = 1;
  REQUIRE_ONCE_DIRECTIVE = 2;

  LITERAL = 100;
}
