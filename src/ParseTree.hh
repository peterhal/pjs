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

