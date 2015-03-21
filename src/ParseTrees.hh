<?hh //strict

class ScriptTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $declarations)
  {
    parent::__construct($range, ParseTreeKind::SCRIPT);
  }
}

class RequireMultipleDirectiveTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $includeFilename)
  {
    parent::__construct(
      $range,
      ParseTreeKind::REQUIRE_MULTIPLE_DIRECTIVE);
  }
}

class RequireOnceDirectiveTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $includeFilename)
  {
    parent::__construct(
      $range,
      ParseTreeKind::REQUIRE_ONCE_DIRECTIVE);
  }
}

class LiteralTree extends ParseTree
{
    public function __construct(
      Range $range,
      public Token $value)
    {
      parent::__construct(
        $range,
        ParseTreeKind::LITERAL);
    }
}

class FunctionDefinitionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public NameToken $name,
    public ParseTree $parameters,
    public ParseTree $returnType,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::FUNCTION_DEFINITION);
  }
}

class ParameterListTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $parameters)
  {
    parent::__construct($range, ParseTreeKind::PARAMETER_LIST);
  }
}

class ParameterDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $type,
    public VariableNameToken $name)
  {
    parent::__construct($range, ParseTreeKind::PARAMETER_DECLARATION);
  }
}

class CompoundStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $statements)
  {
    parent::__construct($range, ParseTreeKind::COMPOUND_STATEMENT);
  }
}

class PredefinedNameTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Token $token)
  {
    parent::__construct($range, ParseTreeKind::PREDEFINED_NAME_TYPE);
  }
}

class KeywordTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Token $token)
  {
    parent::__construct($range, ParseTreeKind::KEYWORD_TYPE);
  }
}

class NullableTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $type)
  {
    parent::__construct($range, ParseTreeKind::NULLABLE_TYPE);
  }
}

class ArrayTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?ParseTree $typeArguments)
  {
    parent::__construct($range, ParseTreeKind::ARRAY_TYPE);
  }
}

class TypeArgumentsTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $types)
  {
    parent::__construct($range, ParseTreeKind::TYPE_ARGUMENTS_TYPE);
  }
}

