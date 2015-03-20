<?hh //strict

class ParseTree
{
  public function __construct(
    private Range $range,
    private ParseTreeKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): ParseTreeKind { return $this->kind; }
}

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

class ThisTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Token $token)
  {
    parent::__construct($range, ParseTreeKind::THIS_TYPE);
  }
}
