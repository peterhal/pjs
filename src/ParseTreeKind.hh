<?hh // strict

enum ParseTreeKind : int
{
  SCRIPT = 0;
  QUALIFIED_NAME = 1;
  REQUIRE_MULTIPLE_DIRECTIVE = 2;
  REQUIRE_ONCE_DIRECTIVE = 3;
  LITERAL = 4;
  ENUM_DECLARATION = 5;
  ENUMERATOR = 6;
  FUNCTION_DEFINITION = 7;
  PARAMETER_LIST = 8;
  PARAMETER_DECLARATION = 9;
  COMPOUND_STATEMENT = 10;
  PREDEFINED_NAME_TYPE = 11;
  KEYWORD_TYPE = 12;
  NULLABLE_TYPE = 13;
  NAMED_TYPE = 14;
  CLOSURE_TYPE = 15;
  TUPLE_TYPE = 16;
  TYPE_ARGUMENTS = 17;
  NAMESPACE_USE_DECLARATION = 18;
  NAMESPACE_USE_CLAUSE = 19;
  NAMESPACE_DEFINITION = 20;
  GENERIC_TYPE_PARAMETER = 21;
  CLASS_DECLARATION = 22;
  TRAIT_USE_CLAUSE = 23;
  CONST_DECLARATION = 24;
  CONST_DECLARATOR = 25;
  PROPERTY_DECLARATOR = 26;
  METHOD_DEFINITION = 27;
  DESTRUCTOR_DECLARATION = 28;
  CONSTRUCTOR_DECLARATION = 29;
  CONSTRUCTOR_PARAMETER = 30;
  INTERFACE_DECLARATION = 31;
  REQUIRES_EXTENDS_CLAUSE = 32;
  REQUIRES_IMPLEMENTS_CLAUSE = 33;
  TRAIT_DECLARATION = 34;
  EMPTY_STATEMENT = 35;
  EXPRESSION_STATEMENT = 36;
  IF_STATEMENT = 37;
  ELSEIF_CLAUSE = 38;
  ELSE_CLAUSE = 39;
  DEFAULT_LABEL = 40;
  CASE_LABEL = 41;
  CASE_CLAUSE = 42;
  SWITCH_STATEMENT = 43;
  WHILE_STATEMENT = 44;
  DO_STATEMENT = 45;
  FOR_STATEMENT = 46;
  ALIAS_EXPRESSION = 47;
  FOR_EACH_STATEMENT = 48;
  CONTINUE_STATEMENT = 49;
  BREAK_STATEMENT = 50;
  RETURN_STATEMENT = 51;
  THROW_STATEMENT = 52;
  TRY_STATEMENT = 53;
  CATCH_CLAUSE = 54;
  FINALLY_CLAUSE = 55;
  YIELD_EXPRESSION = 56;
  ARRAY_ELEMENT_INITIALIZER = 57;
  COLLECTION_LITERAL = 58;
  VARIABLE_NAME = 59;
  SHAPE_LITERAL = 60;
  TUPLE_LITERAL = 61;
  FIELD_INITIALIZER = 62;
  ANONYMOUS_FUNCTION = 63;
  ANONYMOUS_FUNCTION_PARAMETER = 64;
  PARSE_ERROR = 65;
}
