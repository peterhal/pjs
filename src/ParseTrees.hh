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
    public bool $isAsync,
    public NameToken $name,
    public ?Vector<ParseTree> $typeParameters,
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
    public ?Vector<ParseTree> $parameters)
  {
    parent::__construct($range, ParseTreeKind::PARAMETER_LIST);
  }
}

class ParameterDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $type,
    public VariableNameToken $name,
    public ?ParseTree $defaultValue)
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

class NamespaceUseDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $useClauses)
  {
    parent::__construct($range, ParseTreeKind::NAMESPACE_USE_DECLARATION);
  }
}

class NamespaceUseClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $name,
    public ?NameToken $alias)
  {
    parent::__construct($range, ParseTreeKind::NAMESPACE_USE_CLAUSE);
  }
}

class NamespaceDefinitionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?NameToken $name,
    public ?Vector<ParseTree> $declarations)
  {
    parent::__construct($range, ParseTreeKind::NAMESPACE_DEFINITION);
  }
}

class GenericTypeParameterTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?Token $variance,
    public NameToken $name,
    public ?ParseTree $constraint)
  {
    parent::__construct($range, ParseTreeKind::GENERIC_TYPE_PARAMETER);
  }
}

class ClassDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public bool $isAbstract,
    public bool $isFinal,
    public NameToken $name,
    public ?Vector<ParseTree> $typeParameters,
    public ?ParseTree $extendsClause,
    public ?Vector<ParseTree> $implementsClause,
    public Vector<ParseTree> $traits,
    public Vector<ParseTree> $members)
  {
    parent::__construct($range, ParseTreeKind::CLASS_DECLARATION);
  }
}

class TraitUseClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $traits)
  {
    parent::__construct($range, ParseTreeKind::TRAIT_USE_CLAUSE);
  }
}

class ConstDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?ParseTree $type,
    public Vector<ParseTree> $declarators)
  {
    parent::__construct($range, ParseTreeKind::CONST_DECLARATION);
  }
}

class ConstDeclaratorTree extends ParseTree
{
  public function __construct(
    Range $range,
    public NameToken $name,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::CONST_DECLARATOR);
  }
}

class PropertyDeclaratorTree extends ParseTree
{
  public function __construct(
    Range $range,
    public VariableNameToken $name,
    public ?ParseTree $initializer)
  {
    parent::__construct($range, ParseTreeKind::PROPERTY_DECLARATOR);
  }
}

class MethodDefinitionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<Token> $modifiers,
    public NameToken $name,
    public ?Vector<ParseTree> $typeParameters,
    public ParseTree $parameters,
    public ParseTree $returnType,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::METHOD_DEFINITION);
  }
}

class DestructorDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<Token> $modifiers,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::DESTRUCTOR_DECLARATION);
  }
}

class ConstructorDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<Token> $modifiers,
    public ?Vector<ParseTree> $parameters,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::CONSTRUCTOR_DECLARATION);
  }
}

class ConstructorParameterTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<Token> $modifiers,
    public ParseTree $type,
    public VariableNameToken $name,
    public ?ParseTree $defaultValue)
  {
    parent::__construct($range, ParseTreeKind::CONSTRUCTOR_PARAMETER);
  }
}

class InterfaceDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public NameToken $name,
    public ?Vector<ParseTree> $typeParameters,
    public ?Vector<ParseTree> $extendsClause,
    public Vector<ParseTree> $members)
  {
    parent::__construct($range, ParseTreeKind::INTERFACE_DECLARATION);
  }
}

class RequiresExtendsClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $name)
  {
    parent::__construct($range, ParseTreeKind::REQUIRES_EXTENDS_CLAUSE);
  }
}

class RequiresImplementsClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $name)
  {
    parent::__construct($range, ParseTreeKind::REQUIRES_IMPLEMENTS_CLAUSE);
  }
}

class TraitDeclarationTree extends ParseTree
{
  public function __construct(
    Range $range,
    public NameToken $name,
    public ?Vector<ParseTree> $typeParameters,
    public Vector<ParseTree> $traitUseClauses,
    public Vector<ParseTree> $members)
  {
    parent::__construct($range, ParseTreeKind::TRAIT_DECLARATION);
  }
}

class EmptyStatementTree extends ParseTree
{
  public function __construct(
    Range $range)
  {
    parent::__construct($range, ParseTreeKind::EMPTY_STATEMENT);
  }
}

class ExpressionStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $expression)
  {
    parent::__construct($range, ParseTreeKind::EXPRESSION_STATEMENT);
  }
}

class IfStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $condition,
    public ParseTree $thenClause,
    public ?Vector<ParseTree> $elseifClauses,
    public ?ParseTree $elseClause)
  {
    parent::__construct($range, ParseTreeKind::IF_STATEMENT);
  }
}

class ElseifClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $condition,
    public ParseTree $elseClause)
  {
    parent::__construct($range, ParseTreeKind::ELSEIF_CLAUSE);
  }
}

class ElseClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $elseClause)
  {
    parent::__construct($range, ParseTreeKind::ELSE_CLAUSE);
  }
}

class DefaultLabelTree extends ParseTree
{
  public function __construct(
    Range $range)
  {
    parent::__construct($range, ParseTreeKind::DEFAULT_LABEL);
  }
}

class CaseLabelTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $condition)
  {
    parent::__construct($range, ParseTreeKind::CASE_LABEL);
  }
}

class CaseClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $labels,
    public ParseTree $statement)
  {
    parent::__construct($range, ParseTreeKind::CASE_CLAUSE);
  }
}

class SwitchStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $condition,
    public Vector<ParseTree> $caseClauses)
  {
    parent::__construct($range, ParseTreeKind::SWITCH_STATEMENT);
  }
}

class WhileStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $condition,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::WHILE_STATEMENT);
  }
}

class DoStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $body,
    public ParseTree $condition)
  {
    parent::__construct($range, ParseTreeKind::DO_STATEMENT);
  }
}

class ForStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?Vector<ParseTree> $initializer,
    public Vector<ParseTree> $condition,
    public ?Vector<ParseTree> $increment,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::FOR_STATEMENT);
  }
}

class AliasExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $expression)
  {
    parent::__construct($range, ParseTreeKind::ALIAS_EXPRESSION);
  }
}

class ForEachStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $collection,
    public ?ParseTree $key,
    public ParseTree $value,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::FOR_EACH_STATEMENT);
  }
}

class ContinueStatementTree extends ParseTree
{
  public function __construct(
    Range $range)
  {
    parent::__construct($range, ParseTreeKind::CONTINUE_STATEMENT);
  }
}

class BreakStatementTree extends ParseTree
{
  public function __construct(
    Range $range)
  {
    parent::__construct($range, ParseTreeKind::BREAK_STATEMENT);
  }
}

class ReturnStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::RETURN_STATEMENT);
  }
}

class ThrowStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::THROW_STATEMENT);
  }
}

class TryStatementTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $body,
    public Vector<ParseTree> $catchClauses,
    public ?ParseTree $finallyClause)
  {
    parent::__construct($range, ParseTreeKind::TRY_STATEMENT);
  }
}

class CatchClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $type,
    public VariableNameToken $name,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::CATCH_CLAUSE);
  }
}

class FinallyClauseTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::FINALLY_CLAUSE);
  }
}

class YieldExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::YIELD_EXPRESSION);
  }
}

class ArrayElementInitializerTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $key,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::ARRAY_ELEMENT_INITIALIZER);
  }
}

class CollectionLiteralTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $name,
    public Vector<ParseTree> $elements)
  {
    parent::__construct($range, ParseTreeKind::COLLECTION_LITERAL);
  }
}

class VariableNameTree extends ParseTree
{
  public function __construct(
    Range $range,
    public VariableNameToken $name)
  {
    parent::__construct($range, ParseTreeKind::VARIABLE_NAME);
  }
}

class ShapeLiteralTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?Vector<ParseTree> $initializers)
  {
    parent::__construct($range, ParseTreeKind::SHAPE_LITERAL);
  }
}

class TupleLiteralTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Vector<ParseTree> $values)
  {
    parent::__construct($range, ParseTreeKind::TUPLE_LITERAL);
  }
}

class FieldInitializerTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $name,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::FIELD_INITIALIZER);
  }
}

class AnonymousFunctionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public bool $isAsync,
    public ?Vector<ParseTree> $parameters,
    public ?ParseTree $returnType,
    public ?Vector<ParseTree> $useClause,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::ANONYMOUS_FUNCTION);
  }
}

class AnonymousFunctionParameterTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?ParseTree $type,
    public VariableNameToken $name,
    public ?ParseTree $defaultValue)
  {
    parent::__construct($range, ParseTreeKind::ANONYMOUS_FUNCTION_PARAMETER);
  }
}

class ObjectCreationExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?ParseTree $type,
    public ?Vector<ParseTree> $arguments)
  {
    parent::__construct($range, ParseTreeKind::OBJECT_CREATION_EXPRESSION);
  }
}

class ArrayLiteralTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?Vector<ParseTree> $elements)
  {
    parent::__construct($range, ParseTreeKind::ARRAY_LITERAL);
  }
}

class SubscriptOperatorTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $collection,
    public ?ParseTree $index)
  {
    parent::__construct($range, ParseTreeKind::SUBSCRIPT_OPERATOR);
  }
}

class FunctionCallTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $function,
    public ?Vector<ParseTree> $arguments)
  {
    parent::__construct($range, ParseTreeKind::FUNCTION_CALL);
  }
}

class MemberSelectionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $object,
    public NameToken $name)
  {
    parent::__construct($range, ParseTreeKind::MEMBER_SELECTION);
  }
}

class NullSafeMemberSelectionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $object,
    public NameToken $name)
  {
    parent::__construct($range, ParseTreeKind::NULL_SAFE_MEMBER_SELECTION);
  }
}

class PostfixOperatorTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $value,
    public Token $operator)
  {
    parent::__construct($range, ParseTreeKind::POSTFIX_OPERATOR);
  }
}

class ScopeResolutionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $baseName,
    public NameToken $memberName)
  {
    parent::__construct($range, ParseTreeKind::SCOPE_RESOLUTION);
  }
}

class StaticNameTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Token $static)
  {
    parent::__construct($range, ParseTreeKind::STATIC_NAME);
  }
}

class BinaryExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $left,
    public Token $operator,
    public ParseTree $right)
  {
    parent::__construct($range, ParseTreeKind::BINARY_EXPRESSION);
  }
}

class UnaryExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public Token $operator,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::UNARY_EXPRESSION);
  }
}

class CastExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $type,
    public ParseTree $value)
  {
    parent::__construct($range, ParseTreeKind::CAST_EXPRESSION);
  }
}

class ConditionalExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $condition,
    public ?ParseTree $trueVaule,
    public ParseTree $falseValue)
  {
    parent::__construct($range, ParseTreeKind::CONDITIONAL_EXPRESSION);
  }
}

class LambdaExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public bool $isAsync,
    public ParseTree $signature,
    public ParseTree $body)
  {
    parent::__construct($range, ParseTreeKind::LAMBDA_EXPRESSION);
  }
}

class LambdaSignatureTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ?Vector<ParseTree> $parameters,
    public ?ParseTree $returnType)
  {
    parent::__construct($range, ParseTreeKind::LAMBDA_SIGNATURE);
  }
}

class ParenExpressionTree extends ParseTree
{
  public function __construct(
    Range $range,
    public ParseTree $expression)
  {
    parent::__construct($range, ParseTreeKind::PAREN_EXPRESSION);
  }
}

class ParseErrorTree extends ParseTree
{
  public function __construct(
    Range $range)
  {
    parent::__construct($range, ParseTreeKind::PARSE_ERROR);
  }
}
