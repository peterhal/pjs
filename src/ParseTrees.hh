<?hh // strict

require_once 'ParseTree.hh';
require_once 'ParseTreeKind.hh';


class ScriptTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $declarations)
  {
    parent::__construct($range, ParseTreeKind::SCRIPT);
  }
}

class QualifiedNameTree extends ParseTree
{
  public function __construct(
    Range $range,
    public bool $fullyQualified,
    public Vector<NameToken> $names)
  {
    parent::__construct($range, ParseTreeKind::QUALIFIED_NAME);
  }
}

class RequireMultipleDirectiveTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $includeFilename)
  {
    parent::__construct($range, ParseTreeKind::REQUIRE_MULTIPLE_DIRECTIVE);
  }
}

class RequireOnceDirectiveTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $includeFilename)
  {
    parent::__construct($range, ParseTreeKind::REQUIRE_ONCE_DIRECTIVE);
  }
}

class LiteralTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Token $value)
  {
    parent::__construct($range, ParseTreeKind::LITERAL);
  }
}

class EnumDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public NameToken $name,
    public ParseTree $base,
    public ?ParseTree $constraint,
    public Vector<ParseTree> $enumerators)
  {
    parent::__construct($range, ParseTreeKind::ENUM_DECLARATION);
  }
}

class EnumeratorTree extends ParseTree
{
  public function __construct(
    Range $range,
    public NameToken $name,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::ENUMERATOR);
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
    public NameToken $token)
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

class NamedTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $name,
    public ?ParseTree $typeArguments)
  {
    parent::__construct($range, ParseTreeKind::NAMED_TYPE);
  }
}

class ClosureTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?Vector<ParseTree> $parameterTypes,
    public ParseTree $returnType)
  {
    parent::__construct($range, ParseTreeKind::CLOSURE_TYPE);
  }
}

class TupleTypeTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $types)
  {
    parent::__construct($range, ParseTreeKind::TUPLE_TYPE);
  }
}

class TypeArgumentsTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $types)
  {
    parent::__construct($range, ParseTreeKind::TYPE_ARGUMENTS);
  }
}
