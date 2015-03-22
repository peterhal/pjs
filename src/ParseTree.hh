<?hh //strict

require_once 'ParseTreeKind.hh';

class ParseTree
{
  public function __construct(
    private Range $range,
    private ParseTreeKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function kind(): ParseTreeKind { return $this->kind; }
  public function isScript(): bool {
    return $this->kind === ParseTreeKind::SCRIPT;
  }
  public function asScript(): ScriptTree { 
    invariant($this instanceof ScriptTree, "Wrong type.");
    return $this;
  }
  public function isRequireMultipleDirective(): bool {
    return $this->kind === ParseTreeKind::REQUIRE_MULTIPLE_DIRECTIVE;
  }
  public function asRequireMultipleDirective(): RequireMultipleDirectiveTree { 
    invariant($this instanceof RequireMultipleDirectiveTree, "Wrong type.");
    return $this;
  }
  public function isRequireOnceDirective(): bool {
    return $this->kind === ParseTreeKind::REQUIRE_ONCE_DIRECTIVE;
  }
  public function asRequireOnceDirective(): RequireOnceDirectiveTree { 
    invariant($this instanceof RequireOnceDirectiveTree, "Wrong type.");
    return $this;
  }
  public function isLiteral(): bool {
    return $this->kind === ParseTreeKind::LITERAL;
  }
  public function asLiteral(): LiteralTree { 
    invariant($this instanceof LiteralTree, "Wrong type.");
    return $this;
  }
  public function isFunctionDefinition(): bool {
    return $this->kind === ParseTreeKind::FUNCTION_DEFINITION;
  }
  public function asFunctionDefinition(): FunctionDefinitionTree { 
    invariant($this instanceof FunctionDefinitionTree, "Wrong type.");
    return $this;
  }
  public function isParameterList(): bool {
    return $this->kind === ParseTreeKind::PARAMETER_LIST;
  }
  public function asParameterList(): ParameterListTree { 
    invariant($this instanceof ParameterListTree, "Wrong type.");
    return $this;
  }
  public function isParameterDeclaration(): bool {
    return $this->kind === ParseTreeKind::PARAMETER_DECLARATION;
  }
  public function asParameterDeclaration(): ParameterDeclarationTree { 
    invariant($this instanceof ParameterDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isCompoundStatement(): bool {
    return $this->kind === ParseTreeKind::COMPOUND_STATEMENT;
  }
  public function asCompoundStatement(): CompoundStatementTree { 
    invariant($this instanceof CompoundStatementTree, "Wrong type.");
    return $this;
  }
  public function isPredefinedNameType(): bool {
    return $this->kind === ParseTreeKind::PREDEFINED_NAME_TYPE;
  }
  public function asPredefinedNameType(): PredefinedNameTypeTree { 
    invariant($this instanceof PredefinedNameTypeTree, "Wrong type.");
    return $this;
  }
  public function isKeywordType(): bool {
    return $this->kind === ParseTreeKind::KEYWORD_TYPE;
  }
  public function asKeywordType(): KeywordTypeTree { 
    invariant($this instanceof KeywordTypeTree, "Wrong type.");
    return $this;
  }
  public function isNullableType(): bool {
    return $this->kind === ParseTreeKind::NULLABLE_TYPE;
  }
  public function asNullableType(): NullableTypeTree { 
    invariant($this instanceof NullableTypeTree, "Wrong type.");
    return $this;
  }
  public function isArrayType(): bool {
    return $this->kind === ParseTreeKind::ARRAY_TYPE;
  }
  public function asArrayType(): ArrayTypeTree { 
    invariant($this instanceof ArrayTypeTree, "Wrong type.");
    return $this;
  }
  public function isTypeArguments(): bool {
    return $this->kind === ParseTreeKind::TYPE_ARGUMENTS;
  }
  public function asTypeArguments(): TypeArgumentsTree { 
    invariant($this instanceof TypeArgumentsTree, "Wrong type.");
    return $this;
  }
}
