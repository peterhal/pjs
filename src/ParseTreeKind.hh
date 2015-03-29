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
  ARRAY_TYPE = 14;
  NAMED_TYPE = 15;
  CLOSURE_TYPE = 16;
  TUPLE_TYPE = 17;
  TYPE_ARGUMENTS = 18;
  NAMESPACE_USE_DECLARATION = 19;
  NAMESPACE_USE_CLAUSE = 20;
  NAMESPACE_DEFINITION = 21;
  GENERIC_TYPE_PARAMETER = 22;
  CLASS_DECLARATION = 23;
  TRAIT_USE_CLAUSE = 24;
  CONST_DECLARATION = 25;
  CONST_DECLARATOR = 26;
  PROPERTY_DECLARATOR = 27;
  METHOD_DEFINITION = 28;
  DESTRUCTOR_DECLARATION = 29;
  CONSTRUCTOR_DECLARATION = 30;
  CONSTRUCTOR_PARAMETER = 31;
  INTERFACE_DECLARATION = 32;
  REQUIRES_EXTENDS_CLAUSE = 33;
  REQUIRES_IMPLEMENTS_CLAUSE = 34;
  TRAIT_DECLARATION = 35;
  EMPTY_STATEMENT = 36;
  EXPRESSION_STATEMENT = 37;
  IF_STATEMENT = 38;
  ELSEIF_CLAUSE = 39;
  ELSE_CLAUSE = 40;
  DEFAULT_LABEL = 41;
  CASE_LABEL = 42;
  CASE_CLAUSE = 43;
  SWITCH_STATEMENT = 44;
  WHILE_STATEMENT = 45;
  DO_STATEMENT = 46;
  FOR_STATEMENT = 47;
  ALIAS_EXPRESSION = 48;
  FOR_EACH_STATEMENT = 49;
  CONTINUE_STATEMENT = 50;
  BREAK_STATEMENT = 51;
  RETURN_STATEMENT = 52;
  THROW_STATEMENT = 53;
  TRY_STATEMENT = 54;
  CATCH_CLAUSE = 55;
  FINALLY_CLAUSE = 56;
  YIELD_EXPRESSION = 57;
  ARRAY_ELEMENT_INITIALIZER = 58;
  COLLECTION_LITERAL = 59;
  VARIABLE_NAME = 60;
  SHAPE_LITERAL = 61;
  TUPLE_LITERAL = 62;
  FIELD_INITIALIZER = 63;
  ANONYMOUS_FUNCTION = 64;
  ANONYMOUS_FUNCTION_PARAMETER = 65;
  OBJECT_CREATION_EXPRESSION = 66;
  ARRAY_LITERAL = 67;
  SUBSCRIPT_OPERATOR = 68;
  FUNCTION_CALL = 69;
  MEMBER_SELECTION = 70;
  NULL_SAFE_MEMBER_SELECTION = 71;
  POSTFIX_OPERATOR = 72;
  SCOPE_RESOLUTION = 73;
  STATIC_NAME = 74;
  BINARY_EXPRESSION = 75;
  UNARY_EXPRESSION = 76;
  CAST_EXPRESSION = 77;
  CONDITIONAL_EXPRESSION = 78;
  LAMBDA_EXPRESSION = 79;
  LAMBDA_SIGNATURE = 80;
  PAREN_EXPRESSION = 81;
  PARSE_ERROR = 82;
}
