<?hh //strict

namespace Syntax
{

require_once 'Utils/Location.hh';
require_once 'Utils/Range.hh';
require_once 'ParseTreeKind.hh';

use Utils\Location;
use Utils\Range;

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
  public function isArrayType(): bool {
    return $this->kind === ParseTreeKind::ARRAY_TYPE;
  }
  public function asArrayType(): ArrayTypeTree {
    invariant($this instanceof ArrayTypeTree, "Wrong type.");
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
  public function isPropertyDeclaration(): bool {
    return $this->kind === ParseTreeKind::PROPERTY_DECLARATION;
  }
  public function asPropertyDeclaration(): PropertyDeclarationTree {
    invariant($this instanceof PropertyDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isPropertyDeclarator(): bool {
    return $this->kind === ParseTreeKind::PROPERTY_DECLARATOR;
  }
  public function asPropertyDeclarator(): PropertyDeclaratorTree {
    invariant($this instanceof PropertyDeclaratorTree, "Wrong type.");
    return $this;
  }
  public function isMethodDefinition(): bool {
    return $this->kind === ParseTreeKind::METHOD_DEFINITION;
  }
  public function asMethodDefinition(): MethodDefinitionTree {
    invariant($this instanceof MethodDefinitionTree, "Wrong type.");
    return $this;
  }
  public function isDestructorDeclaration(): bool {
    return $this->kind === ParseTreeKind::DESTRUCTOR_DECLARATION;
  }
  public function asDestructorDeclaration(): DestructorDeclarationTree {
    invariant($this instanceof DestructorDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isConstructorDeclaration(): bool {
    return $this->kind === ParseTreeKind::CONSTRUCTOR_DECLARATION;
  }
  public function asConstructorDeclaration(): ConstructorDeclarationTree {
    invariant($this instanceof ConstructorDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isConstructorParameter(): bool {
    return $this->kind === ParseTreeKind::CONSTRUCTOR_PARAMETER;
  }
  public function asConstructorParameter(): ConstructorParameterTree {
    invariant($this instanceof ConstructorParameterTree, "Wrong type.");
    return $this;
  }
  public function isInterfaceDeclaration(): bool {
    return $this->kind === ParseTreeKind::INTERFACE_DECLARATION;
  }
  public function asInterfaceDeclaration(): InterfaceDeclarationTree {
    invariant($this instanceof InterfaceDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isRequiresExtendsClause(): bool {
    return $this->kind === ParseTreeKind::REQUIRES_EXTENDS_CLAUSE;
  }
  public function asRequiresExtendsClause(): RequiresExtendsClauseTree {
    invariant($this instanceof RequiresExtendsClauseTree, "Wrong type.");
    return $this;
  }
  public function isRequiresImplementsClause(): bool {
    return $this->kind === ParseTreeKind::REQUIRES_IMPLEMENTS_CLAUSE;
  }
  public function asRequiresImplementsClause(): RequiresImplementsClauseTree {
    invariant($this instanceof RequiresImplementsClauseTree, "Wrong type.");
    return $this;
  }
  public function isTraitDeclaration(): bool {
    return $this->kind === ParseTreeKind::TRAIT_DECLARATION;
  }
  public function asTraitDeclaration(): TraitDeclarationTree {
    invariant($this instanceof TraitDeclarationTree, "Wrong type.");
    return $this;
  }
  public function isEmptyStatement(): bool {
    return $this->kind === ParseTreeKind::EMPTY_STATEMENT;
  }
  public function asEmptyStatement(): EmptyStatementTree {
    invariant($this instanceof EmptyStatementTree, "Wrong type.");
    return $this;
  }
  public function isExpressionStatement(): bool {
    return $this->kind === ParseTreeKind::EXPRESSION_STATEMENT;
  }
  public function asExpressionStatement(): ExpressionStatementTree {
    invariant($this instanceof ExpressionStatementTree, "Wrong type.");
    return $this;
  }
  public function isIfStatement(): bool {
    return $this->kind === ParseTreeKind::IF_STATEMENT;
  }
  public function asIfStatement(): IfStatementTree {
    invariant($this instanceof IfStatementTree, "Wrong type.");
    return $this;
  }
  public function isElseifClause(): bool {
    return $this->kind === ParseTreeKind::ELSEIF_CLAUSE;
  }
  public function asElseifClause(): ElseifClauseTree {
    invariant($this instanceof ElseifClauseTree, "Wrong type.");
    return $this;
  }
  public function isElseClause(): bool {
    return $this->kind === ParseTreeKind::ELSE_CLAUSE;
  }
  public function asElseClause(): ElseClauseTree {
    invariant($this instanceof ElseClauseTree, "Wrong type.");
    return $this;
  }
  public function isDefaultLabel(): bool {
    return $this->kind === ParseTreeKind::DEFAULT_LABEL;
  }
  public function asDefaultLabel(): DefaultLabelTree {
    invariant($this instanceof DefaultLabelTree, "Wrong type.");
    return $this;
  }
  public function isCaseLabel(): bool {
    return $this->kind === ParseTreeKind::CASE_LABEL;
  }
  public function asCaseLabel(): CaseLabelTree {
    invariant($this instanceof CaseLabelTree, "Wrong type.");
    return $this;
  }
  public function isCaseClause(): bool {
    return $this->kind === ParseTreeKind::CASE_CLAUSE;
  }
  public function asCaseClause(): CaseClauseTree {
    invariant($this instanceof CaseClauseTree, "Wrong type.");
    return $this;
  }
  public function isSwitchStatement(): bool {
    return $this->kind === ParseTreeKind::SWITCH_STATEMENT;
  }
  public function asSwitchStatement(): SwitchStatementTree {
    invariant($this instanceof SwitchStatementTree, "Wrong type.");
    return $this;
  }
  public function isWhileStatement(): bool {
    return $this->kind === ParseTreeKind::WHILE_STATEMENT;
  }
  public function asWhileStatement(): WhileStatementTree {
    invariant($this instanceof WhileStatementTree, "Wrong type.");
    return $this;
  }
  public function isDoStatement(): bool {
    return $this->kind === ParseTreeKind::DO_STATEMENT;
  }
  public function asDoStatement(): DoStatementTree {
    invariant($this instanceof DoStatementTree, "Wrong type.");
    return $this;
  }
  public function isForStatement(): bool {
    return $this->kind === ParseTreeKind::FOR_STATEMENT;
  }
  public function asForStatement(): ForStatementTree {
    invariant($this instanceof ForStatementTree, "Wrong type.");
    return $this;
  }
  public function isAliasExpression(): bool {
    return $this->kind === ParseTreeKind::ALIAS_EXPRESSION;
  }
  public function asAliasExpression(): AliasExpressionTree {
    invariant($this instanceof AliasExpressionTree, "Wrong type.");
    return $this;
  }
  public function isForEachStatement(): bool {
    return $this->kind === ParseTreeKind::FOR_EACH_STATEMENT;
  }
  public function asForEachStatement(): ForEachStatementTree {
    invariant($this instanceof ForEachStatementTree, "Wrong type.");
    return $this;
  }
  public function isContinueStatement(): bool {
    return $this->kind === ParseTreeKind::CONTINUE_STATEMENT;
  }
  public function asContinueStatement(): ContinueStatementTree {
    invariant($this instanceof ContinueStatementTree, "Wrong type.");
    return $this;
  }
  public function isBreakStatement(): bool {
    return $this->kind === ParseTreeKind::BREAK_STATEMENT;
  }
  public function asBreakStatement(): BreakStatementTree {
    invariant($this instanceof BreakStatementTree, "Wrong type.");
    return $this;
  }
  public function isReturnStatement(): bool {
    return $this->kind === ParseTreeKind::RETURN_STATEMENT;
  }
  public function asReturnStatement(): ReturnStatementTree {
    invariant($this instanceof ReturnStatementTree, "Wrong type.");
    return $this;
  }
  public function isThrowStatement(): bool {
    return $this->kind === ParseTreeKind::THROW_STATEMENT;
  }
  public function asThrowStatement(): ThrowStatementTree {
    invariant($this instanceof ThrowStatementTree, "Wrong type.");
    return $this;
  }
  public function isTryStatement(): bool {
    return $this->kind === ParseTreeKind::TRY_STATEMENT;
  }
  public function asTryStatement(): TryStatementTree {
    invariant($this instanceof TryStatementTree, "Wrong type.");
    return $this;
  }
  public function isCatchClause(): bool {
    return $this->kind === ParseTreeKind::CATCH_CLAUSE;
  }
  public function asCatchClause(): CatchClauseTree {
    invariant($this instanceof CatchClauseTree, "Wrong type.");
    return $this;
  }
  public function isFinallyClause(): bool {
    return $this->kind === ParseTreeKind::FINALLY_CLAUSE;
  }
  public function asFinallyClause(): FinallyClauseTree {
    invariant($this instanceof FinallyClauseTree, "Wrong type.");
    return $this;
  }
  public function isYieldExpression(): bool {
    return $this->kind === ParseTreeKind::YIELD_EXPRESSION;
  }
  public function asYieldExpression(): YieldExpressionTree {
    invariant($this instanceof YieldExpressionTree, "Wrong type.");
    return $this;
  }
  public function isArrayElementInitializer(): bool {
    return $this->kind === ParseTreeKind::ARRAY_ELEMENT_INITIALIZER;
  }
  public function asArrayElementInitializer(): ArrayElementInitializerTree {
    invariant($this instanceof ArrayElementInitializerTree, "Wrong type.");
    return $this;
  }
  public function isCollectionLiteral(): bool {
    return $this->kind === ParseTreeKind::COLLECTION_LITERAL;
  }
  public function asCollectionLiteral(): CollectionLiteralTree {
    invariant($this instanceof CollectionLiteralTree, "Wrong type.");
    return $this;
  }
  public function isVariableName(): bool {
    return $this->kind === ParseTreeKind::VARIABLE_NAME;
  }
  public function asVariableName(): VariableNameTree {
    invariant($this instanceof VariableNameTree, "Wrong type.");
    return $this;
  }
  public function isShapeLiteral(): bool {
    return $this->kind === ParseTreeKind::SHAPE_LITERAL;
  }
  public function asShapeLiteral(): ShapeLiteralTree {
    invariant($this instanceof ShapeLiteralTree, "Wrong type.");
    return $this;
  }
  public function isTupleLiteral(): bool {
    return $this->kind === ParseTreeKind::TUPLE_LITERAL;
  }
  public function asTupleLiteral(): TupleLiteralTree {
    invariant($this instanceof TupleLiteralTree, "Wrong type.");
    return $this;
  }
  public function isFieldInitializer(): bool {
    return $this->kind === ParseTreeKind::FIELD_INITIALIZER;
  }
  public function asFieldInitializer(): FieldInitializerTree {
    invariant($this instanceof FieldInitializerTree, "Wrong type.");
    return $this;
  }
  public function isAnonymousFunction(): bool {
    return $this->kind === ParseTreeKind::ANONYMOUS_FUNCTION;
  }
  public function asAnonymousFunction(): AnonymousFunctionTree {
    invariant($this instanceof AnonymousFunctionTree, "Wrong type.");
    return $this;
  }
  public function isAnonymousFunctionParameter(): bool {
    return $this->kind === ParseTreeKind::ANONYMOUS_FUNCTION_PARAMETER;
  }
  public function asAnonymousFunctionParameter(): AnonymousFunctionParameterTree {
    invariant($this instanceof AnonymousFunctionParameterTree, "Wrong type.");
    return $this;
  }
  public function isObjectCreationExpression(): bool {
    return $this->kind === ParseTreeKind::OBJECT_CREATION_EXPRESSION;
  }
  public function asObjectCreationExpression(): ObjectCreationExpressionTree {
    invariant($this instanceof ObjectCreationExpressionTree, "Wrong type.");
    return $this;
  }
  public function isArrayLiteral(): bool {
    return $this->kind === ParseTreeKind::ARRAY_LITERAL;
  }
  public function asArrayLiteral(): ArrayLiteralTree {
    invariant($this instanceof ArrayLiteralTree, "Wrong type.");
    return $this;
  }
  public function isSubscriptOperator(): bool {
    return $this->kind === ParseTreeKind::SUBSCRIPT_OPERATOR;
  }
  public function asSubscriptOperator(): SubscriptOperatorTree {
    invariant($this instanceof SubscriptOperatorTree, "Wrong type.");
    return $this;
  }
  public function isFunctionCall(): bool {
    return $this->kind === ParseTreeKind::FUNCTION_CALL;
  }
  public function asFunctionCall(): FunctionCallTree {
    invariant($this instanceof FunctionCallTree, "Wrong type.");
    return $this;
  }
  public function isMemberSelection(): bool {
    return $this->kind === ParseTreeKind::MEMBER_SELECTION;
  }
  public function asMemberSelection(): MemberSelectionTree {
    invariant($this instanceof MemberSelectionTree, "Wrong type.");
    return $this;
  }
  public function isNullSafeMemberSelection(): bool {
    return $this->kind === ParseTreeKind::NULL_SAFE_MEMBER_SELECTION;
  }
  public function asNullSafeMemberSelection(): NullSafeMemberSelectionTree {
    invariant($this instanceof NullSafeMemberSelectionTree, "Wrong type.");
    return $this;
  }
  public function isPostfixOperator(): bool {
    return $this->kind === ParseTreeKind::POSTFIX_OPERATOR;
  }
  public function asPostfixOperator(): PostfixOperatorTree {
    invariant($this instanceof PostfixOperatorTree, "Wrong type.");
    return $this;
  }
  public function isScopeResolution(): bool {
    return $this->kind === ParseTreeKind::SCOPE_RESOLUTION;
  }
  public function asScopeResolution(): ScopeResolutionTree {
    invariant($this instanceof ScopeResolutionTree, "Wrong type.");
    return $this;
  }
  public function isStaticName(): bool {
    return $this->kind === ParseTreeKind::STATIC_NAME;
  }
  public function asStaticName(): StaticNameTree {
    invariant($this instanceof StaticNameTree, "Wrong type.");
    return $this;
  }
  public function isBinaryExpression(): bool {
    return $this->kind === ParseTreeKind::BINARY_EXPRESSION;
  }
  public function asBinaryExpression(): BinaryExpressionTree {
    invariant($this instanceof BinaryExpressionTree, "Wrong type.");
    return $this;
  }
  public function isUnaryExpression(): bool {
    return $this->kind === ParseTreeKind::UNARY_EXPRESSION;
  }
  public function asUnaryExpression(): UnaryExpressionTree {
    invariant($this instanceof UnaryExpressionTree, "Wrong type.");
    return $this;
  }
  public function isCastExpression(): bool {
    return $this->kind === ParseTreeKind::CAST_EXPRESSION;
  }
  public function asCastExpression(): CastExpressionTree {
    invariant($this instanceof CastExpressionTree, "Wrong type.");
    return $this;
  }
  public function isConditionalExpression(): bool {
    return $this->kind === ParseTreeKind::CONDITIONAL_EXPRESSION;
  }
  public function asConditionalExpression(): ConditionalExpressionTree {
    invariant($this instanceof ConditionalExpressionTree, "Wrong type.");
    return $this;
  }
  public function isLambdaExpression(): bool {
    return $this->kind === ParseTreeKind::LAMBDA_EXPRESSION;
  }
  public function asLambdaExpression(): LambdaExpressionTree {
    invariant($this instanceof LambdaExpressionTree, "Wrong type.");
    return $this;
  }
  public function isLambdaSignature(): bool {
    return $this->kind === ParseTreeKind::LAMBDA_SIGNATURE;
  }
  public function asLambdaSignature(): LambdaSignatureTree {
    invariant($this instanceof LambdaSignatureTree, "Wrong type.");
    return $this;
  }
  public function isParenExpression(): bool {
    return $this->kind === ParseTreeKind::PAREN_EXPRESSION;
  }
  public function asParenExpression(): ParenExpressionTree {
    invariant($this instanceof ParenExpressionTree, "Wrong type.");
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
}
