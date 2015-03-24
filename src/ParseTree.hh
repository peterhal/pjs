<?hh //strict

require_once 'ParseTreeKind.hh';

class ParseTree
{
  public function __construct(
    private Range $range,
    private ParseTreeKind $kind)
  {}
  public function range(): Range { return $this->range; }
  public function start(): Location { return $this->range->start(); }
  public function kind(): ParseTreeKind { return $this->kind; }
  public function isScript(): bool {
    return $this->kind === ParseTreeKind::SCRIPT;
  }
  public function asScript(): ScriptTree {
    invariant($this instanceof ScriptTree, "Wrong type.");
    return $this;
  }
  public function isQualifiedName(): bool {
    return $this->kind === ParseTreeKind::QUALIFIED_NAME;
  }
  public function asQualifiedName(): QualifiedNameTree {
    invariant($this instanceof QualifiedNameTree, "Wrong type.");
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
  public function isEnumDeclaration(): bool {
    return $this->kind === ParseTreeKind::ENUM_DECLARATION;
  }
  public function asEnumDeclaration(): EnumDeclarationTree {
    invariant($this instanceof EnumDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isEnumerator(): bool {
    return $this->kind === ParseTreeKind::ENUMERATOR;
  }
  public function asEnumerator(): EnumeratorTree {
    invariant($this instanceof EnumeratorTree, "Wrong type.");
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
  public function isNamedType(): bool {
    return $this->kind === ParseTreeKind::NAMED_TYPE;
  }
  public function asNamedType(): NamedTypeTree {
    invariant($this instanceof NamedTypeTree, "Wrong type.");
    return $this;
  }
  public function isClosureType(): bool {
    return $this->kind === ParseTreeKind::CLOSURE_TYPE;
  }
  public function asClosureType(): ClosureTypeTree {
    invariant($this instanceof ClosureTypeTree, "Wrong type.");
    return $this;
  }
  public function isTupleType(): bool {
    return $this->kind === ParseTreeKind::TUPLE_TYPE;
  }
  public function asTupleType(): TupleTypeTree {
    invariant($this instanceof TupleTypeTree, "Wrong type.");
    return $this;
  }
  public function isTypeArguments(): bool {
    return $this->kind === ParseTreeKind::TYPE_ARGUMENTS;
  }
  public function asTypeArguments(): TypeArgumentsTree {
    invariant($this instanceof TypeArgumentsTree, "Wrong type.");
    return $this;
  }
  public function isNamespaceUseDeclaration(): bool {
    return $this->kind === ParseTreeKind::NAMESPACE_USE_DECLARATION;
  }
  public function asNamespaceUseDeclaration(): NamespaceUseDeclarationTree {
    invariant($this instanceof NamespaceUseDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isNamespaceUseClause(): bool {
    return $this->kind === ParseTreeKind::NAMESPACE_USE_CLAUSE;
  }
  public function asNamespaceUseClause(): NamespaceUseClauseTree {
    invariant($this instanceof NamespaceUseClauseTree, "Wrong type.");
    return $this;
  }
  public function isNamespaceDefinition(): bool {
    return $this->kind === ParseTreeKind::NAMESPACE_DEFINITION;
  }
  public function asNamespaceDefinition(): NamespaceDefinitionTree {
    invariant($this instanceof NamespaceDefinitionTree, "Wrong type.");
    return $this;
  }
  public function isGenericTypeParameter(): bool {
    return $this->kind === ParseTreeKind::GENERIC_TYPE_PARAMETER;
  }
  public function asGenericTypeParameter(): GenericTypeParameterTree {
    invariant($this instanceof GenericTypeParameterTree, "Wrong type.");
    return $this;
  }
  public function isClassDeclaration(): bool {
    return $this->kind === ParseTreeKind::CLASS_DECLARATION;
  }
  public function asClassDeclaration(): ClassDeclarationTree {
    invariant($this instanceof ClassDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isTraitUseClause(): bool {
    return $this->kind === ParseTreeKind::TRAIT_USE_CLAUSE;
  }
  public function asTraitUseClause(): TraitUseClauseTree {
    invariant($this instanceof TraitUseClauseTree, "Wrong type.");
    return $this;
  }
  public function isConstDeclaration(): bool {
    return $this->kind === ParseTreeKind::CONST_DECLARATION;
  }
  public function asConstDeclaration(): ConstDeclarationTree {
    invariant($this instanceof ConstDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isConstDeclarator(): bool {
    return $this->kind === ParseTreeKind::CONST_DECLARATOR;
  }
  public function asConstDeclarator(): ConstDeclaratorTree {
    invariant($this instanceof ConstDeclaratorTree, "Wrong type.");
    return $this;
  }
  public function isParseError(): bool {
    return $this->kind === ParseTreeKind::PARSE_ERROR;
  }
  public function asParseError(): ParseErrorTree {
    invariant($this instanceof ParseErrorTree, "Wrong type.");
    return $this;
  }
}
