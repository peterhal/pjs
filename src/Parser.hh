<?hh //strict

require_once 'ParseTreeKind.hh';
require_once 'ParseTree.hh';
require_once 'ParseTrees.hh';
require_once 'ParserBase.hh';
require_once 'PredefinedName.hh';
require_once 'Token.hh';
require_once 'TokenKind.hh';

class Parser extends ParserBase
{
  public function __construct(
      protected Vector<Token> $tokens,
      protected ErrorReporter $reporter)
  {
    parent::__construct($tokens, $reporter);
  }

  public static function parse(
    Vector<Token> $tokens,
    ErrorReporter $reporter): ParseTree
  {
    $parser = new Parser($tokens, $reporter);
    return $parser->parseScript();
  }

  public function parseScript(): ParseTree
  {
    $start = $this->position();

    // <?hh
    $this->eat(TokenKind::OPEN_ANGLE);
    $this->eat(TokenKind::QUESTION);
    $this->eatPredefinedName(PredefinedName::hh);
    // TODO: Check for whitespace

    // TODO: CHeck for //strict

    $declarations = $this->parseDeclarationList();
    return new ScriptTree(
        $this->getRange($start),
        $declarations);
  }

  public function parseDeclarationList(): Vector<ParseTree>
  {
    $result = Vector {};

    while (!$this->eof()) {
      if (!$this->peekDeclaration()) {
        $this->error("Expected declaration.");
        break;
      }
      $result[] = $this->parseDeclaration();
    }

    return $result;
  }

  public function peekDeclaration(): bool
  {
    switch ($this->peek()) {
    case TokenKind::KW_REQUIRE:
    case TokenKind::KW_REQUIRE_ONCE:
    case TokenKind::KW_FUNCTION:
    case TokenKind::KW_ASYNC:
    case TokenKind::LEFT_SHIFT: // attributes
    case TokenKind::KW_ENUM:
    case TokenKind::KW_NAMESPACE:
    case TokenKind::KW_USE:
    case TokenKind::KW_ABSTRACT:
    case TokenKind::KW_FINAL:
    case TokenKind::KW_CLASS:
      return true;
    default:
      // TODO
      return false;
    }
  }

  public function parseDeclaration(): ParseTree
  {
    switch ($this->peek()) {
    case TokenKind::KW_REQUIRE:
      return $this->parseRequireMultipleDirective();
    case TokenKind::KW_REQUIRE_ONCE:
      return $this->parseRequireOnceDirective();
    case TokenKind::KW_ASYNC:
    case TokenKind::KW_FUNCTION:
      return $this->parseFunctionDefinition();
    case TokenKind::LEFT_SHIFT:
      // TODO: attributes
      throw new Exception();
    case TokenKind::KW_ENUM:
      return $this->parseEnum();
    case TokenKind::KW_NAMESPACE:
      return $this->parseNamespace();
    case TokenKind::KW_USE:
      return $this->parseNamespaceUseDeclaration();
    case TokenKind::KW_ABSTRACT:
    case TokenKind::KW_FINAL:
    case TokenKind::KW_CLASS:
      return $this->parseClassDeclaration();
    default:
      // TODO
      throw new Exception();
    }
  }

  public function parseRequireOnceDirective(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_REQUIRE_ONCE);
    $includeFilename = $this->parseExpression();
    // TODO: SPEC missing ; in grammar
    $this->eat(TokenKind::SEMI_COLON);

    return new RequireOnceDirectiveTree(
      $this->getRange($start),
      $includeFilename);
  }

  public function parseRequireMultipleDirective(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_REQUIRE);
    $includeFilename = $this->parseExpression();
    // TODO: SPEC missing ; in grammar
    $this->eat(TokenKind::SEMI_COLON);

    return new RequireMultipleDirectiveTree(
      $this->getRange($start),
      $includeFilename);
  }
  public function parseNamespaceUseDeclaration(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_USE);
    $useClauses = $this->parseNamespaceUseClauses();
    $this->eat(TokenKind::SEMI_COLON);

    return new NamespaceUseDeclarationTree(
      $this->getRange($start),
      $useClauses);
  }

  private function parseNamespaceUseClauses(): Vector<ParseTree>
  {
    $useClauses = Vector {};
    $useClauses[] = $this->parseNamespaceUseClause();
    while ($this->eatOpt(TokenKind::COMMA)) {
      $useClauses[] = $this->parseNamespaceUseClause();
    }
    return $useClauses;
  }

  private function parseNamespaceUseClause(): ParseTree
  {
    $start = $this->position();

    $name = $this->parseQualifiedName();
    if ($this->eatOpt(TokenKind::KW_AS)) {
      $alias = $this->eatName();
      return new NamespaceUseClauseTree(
        $this->getRange($start),
        $name,
        $alias);
    } else {
      return new NamespaceUseClauseTree(
        $this->getRange($start),
        $name,
        null);
    }
  }

  public function parseNamespace(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_NAMESPACE);
    if ($this->peekKind(TokenKind::NAME)) {
      $name = $this->eatName();
      if ($this->eatOpt(TokenKind::SEMI_COLON)) {
        return new NamespaceDefinitionTree(
          $this->getRange($start),
          $name,
          null);
      } else {
        $this->eat(TokenKind::OPEN_CURLY);
        $declarations = $this->parseDeclarationList();
        $this->eat(TokenKind::CLOSE_CURLY);

        return new NamespaceDefinitionTree(
          $this->getRange($start),
          $name,
          $declarations);
      }
    } else {
        $this->eat(TokenKind::OPEN_CURLY);
        $declarations = $this->parseDeclarationList();
        $this->eat(TokenKind::CLOSE_CURLY);

        return new NamespaceDefinitionTree(
          $this->getRange($start),
          null,
          $declarations);
    }
  }

  public function parseEnum(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_ENUM);
    $name = $this->eatName();

    $this->eat(TokenKind::COLON);
    $base = $this->parseTypeSpecifier();
    if (!Parser::isValidEnumBase($base)) {
      $this->errorLocation(
        $base->start(), 
        "Invalid enum base. Expected 'int' or 'string'.");
    }

    if ($this->eatOpt(TokenKind::KW_AS)) {
      $constraint = $this->parseTypeSpecifier();
    } else {
      $constraint = null;
    }

    $this->eat(TokenKind::OPEN_CURLY);
    $enumerators = $this->parseEnumeratorList();
    $this->eat(TokenKind::CLOSE_CURLY);

    return new EnumDeclarationTree(
      $this->getRange($start),
      $name,
      $base,
      $constraint,
      $enumerators);
  }

  private static function isValidEnumBase(ParseTree $value): bool
  {
    if (!$value->isPredefinedNameType()) {
      return false;
    }
    $name = $value->asPredefinedNameType()->token->value();
    return $name === PredefinedName::string || $name == PredefinedName::int;
  }

  private function parseEnumeratorList(): Vector<ParseTree>
  {
    $result = Vector {};
    $result[] = $this->parseEnumerator();
    while ($this->eatOpt(TokenKind::SEMI_COLON)) {
      $result[] = $this->parseEnumerator();
    }
    return $result;
  }

  private function parseEnumerator(): ParseTree
  {
    $start = $this->position();

    $name = $this->eatName();
    $this->eat(TokenKind::EQUAL);
    $value = $this->parseExpression();

    return new EnumeratorTree(
      $this->getRange($start),
      $name,
      $value);
  }

  private function parseClassDeclaration(): ParseTree
  {
    $start = $this->position();

    $isAbstract = $this->eatOpt(TokenKind::KW_ABSTRACT);
    $isFinal = $this->eatOpt(TokenKind::KW_FINAL);
    if ($isAbstract && $isFinal) {
      $this->error("Class cannot be both 'abstract' and 'final'.");
    }

    $this->eat(TokenKind::KW_CLASS);
    $name = $this->eatName();
    $typeParameters = $this->parseTypeParametersOpt();
    if ($this->eatOpt(TokenKind::KW_EXTENDS)) {
      $extendsClause = $this->parseQualifiedNameType();
    } else {
      $extendsClause = null;
    }
    if ($this->eatOpt(TokenKind::KW_IMPLEMENTS)) {
      $implementsClause = Vector {};
      $implementsClause[] = $this->parseQualifiedNameType();
      while ($this->eatOpt(TokenKind::COMMA)) {
        $implementsClause[] = $this->parseQualifiedNameType();
      }
    } else {
      $implementsClause = null;
    }
    $traits = $this->parseTraitUseClauses();
    // TODO:
    $members = Vector {};

    return new ClassDeclarationTree(
      $this->getRange($start),
      $isAbstract,
      $isFinal,
      $name,
      $typeParameters,
      $extendsClause,
      $implementsClause,
      $traits,
      $members);
  }

  private function parseTraitUseClauses(): Vector<ParseTree>
  {
    $result = Vector {};
    while ($this->peekKind(TokenKind::KW_USE)) {
      $result[] = $this->parseTraitUseClause();
    }
    return $result;
  }

  private function parseTraitUseClause(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_USE);

    $traits = Vector {};
    $traits[] = $this->parseQualifiedNameType();
    while ($this->eatOpt(TokenKind::COMMA)) {
      $traits[] = $this->parseQualifiedNameType();
    }

    return new TraitUseClauseTree(
      $this->getRange($start),
      $traits);
  }

  private function parseTypeParametersOpt(): ?Vector<ParseTree>
  {
    if (!$this->peekKind(TokenKind::OPEN_ANGLE)) {
      return null;
    } else {
      $start = $this->position();

      $this->eat(TokenKind::OPEN_ANGLE);
      $result = Vector {};
      $result[] = $this->parseTypeParameter();
      while ($this->eatOpt(TokenKind::COMMA)) {
        $result[] = $this->parseTypeParameter();
      }
      $this->eat(TokenKind::CLOSE_ANGLE);

      return $result;
    }
  }

  private function parseTypeParameter(): ParseTree
  {
    $start = $this->position();

    $variance = $this->parseVarianceOpt();
    $name = $this->eatName();
    if ($this->eatOpt(TokenKind::KW_AS)) {
      $constraint = $this->parseTypeSpecifier();
    } else {
      $constraint = null;
    }

    return new GenericTypeParameterTree(
      $this->getRange($start),
      $variance,
      $name,
      $constraint);
  }

  private function parseVarianceOpt(): ?Token
  {
    switch ($this->peek()) {
    case TokenKind::PLUS:
    case TokenKind::MINUS:
      return $this->next();
    default:
      return null;
    }
  }

  private function parseExpression(): ParseTree
  {
    $start = $this->position();

    // TODO
    $value = $this->eat(TokenKind::SINGLE_QUOTED_STRING);

    return new LiteralTree($this->getRange($start) , $value);
  }

  public function parseFunctionDefinition(): ParseTree
  {
    $start = $this->position();

    // TODO: attributes
    // TODO: async
    $this->eat(TokenKind::KW_FUNCTION);
    $name = $this->eatName();
    // TODO: type parameters
    $parameters = $this->parseParameterList();
    $this->eat(TokenKind::COLON);
    $returnType = $this->parseReturnType();
    $body = $this->parseCompoundStatement();

    return new FunctionDefinitionTree(
      $this->getRange($start),
      $name,
      $parameters,
      $returnType,
      $body);
  }

  public function parseParameterList(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::OPEN_PAREN);

    $parameters = Vector {};
    if ($this->peekParameter()) {
      $parameters[] = $this->parseParameterDeclaration();
      while ($this->eatOpt(TokenKind::COMMA)) {
        $parameters[] = $this->parseParameterDeclaration();
      }
    }

    $this->eat(TokenKind::CLOSE_PAREN);

    return new ParameterListTree(
      $this->getRange($start),
      $parameters);
  }

  private function peekParameter(): bool
  {
    // TODO: attribute
    return $this->peekTypeSpecifier();
  }

  private function parseParameterDeclaration(): ParseTree
  {
    $start = $this->position();

    // TODO: Attribute
    $type = $this->parseTypeSpecifier();
    $name = $this->eatVariableName();
    // TODO: default argument specifier

    return new ParameterDeclarationTree(
      $this->getRange($start),
      $type,
      $name);
  }

  public function parseCompoundStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::OPEN_CURLY);
    $statements = Vector {};
    while ($this->peekStatement()) {
      $statements[] = $this->parseStatement();
    }
    $this->eat(TokenKind::CLOSE_CURLY);

    return new CompoundStatementTree(
      $this->getRange($start),
      $statements);
  }

  private function peekStatement(): bool
  {
    // TODO
    return false;
  }

  private function parseStatement(): ParseTree
  {
    $start = $this->position();

    return new CompoundStatementTree(
      $this->getRange($start),
      Vector {});
  }

  private function peekTypeSpecifier(): bool
  {
    switch ($this->peek()) {
    case TokenKind::KW_ARRAYKEY:
    case TokenKind::KW_NUM:
    case TokenKind::OPEN_PAREN: // tuple, closure
    case TokenKind::QUESTION: // nullable
    case TokenKind::BACK_SLASH: // qualified name
      return true;
    case TokenKind::NAME: // qualified name, type parameter
      // array
      // bool
      // int`
      // float
      // mixed
      // resource
      // string
      // void
      return true;
    default:
      return false;
    }
  }

  private function parseQualifiedName(): ParseTree
  {
    $start = $this->position();

    $fullyQualified = $this->eatOpt(TokenKind::BACK_SLASH);
    $names = Vector {};

    do
    {
      $names[] = $this->eatName();
    } while ($this->eatOpt(TokenKind::BACK_SLASH));

    return new QualifiedNameTree($this->getRange($start), $fullyQualified, $names);
  }

  private function parseTypeSpecifier(): ParseTree
  {
    switch ($this->peek()) {
    case TokenKind::KW_ARRAYKEY:
    case TokenKind::KW_NUM:
      return $this->parseKeywordType();
    case TokenKind::OPEN_PAREN:
      if ($this->peekIndex(1) === TokenKind::KW_FUNCTION) {
        return $this->parseClosureType();
      } else {
        return $this->parseTupleType();
      }
    case TokenKind::QUESTION:
      return $this->parseNullableType();
    case TokenKind::BACK_SLASH:
    case TokenKind::NAME:
      return $this->parseNamedType();
    default:
      throw new Exception();
    }
  }

  private function parseNamedType(): ParseTree
  {
    if ($this->peekKind(TokenKind::NAME)) {
      $start = $this->position();

      $value = $this->peekToken()->asName()->value();

      switch ($value) {
      case PredefinedName::bool:
      case PredefinedName::int:
      case PredefinedName::float:
      case PredefinedName::mixed:
      case PredefinedName::pn_resource:
      case PredefinedName::string:
      case PredefinedName::void:
        $name = $this->eatName();
        return new PredefinedNameTypeTree($this->getRange($start), $name);
      case PredefinedName::pn_array:
        // Parse as regular possibly generic class.
        break;
      }
    }

    return $this->parseQualifiedNameType();
  }

  private function parseQualifiedNameType(): ParseTree
  {
    $start = $this->position();

    $name = $this->parseQualifiedName();
    $typeArguments = $this->parseTypeArgumentsOpt();
    return new NamedTypeTree($this->getRange($start), $name, $typeArguments);
  }

  private function parseTupleType(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::OPEN_PAREN);

    $types = Vector {};

    $types[] = $this->parseTypeSpecifier();
    $this->eat(TokenKind::COMMA);
    $types = $this->parseTypeSpecifierList($types);
    $this->eat(TokenKind::CLOSE_PAREN);

    return new TupleTypeTree($this->getRange($start), $types);
  }

  private function parseClosureType(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::OPEN_PAREN);
    $this->eat(TokenKind::KW_FUNCTION);
    $this->eat(TokenKind::OPEN_PAREN);
    $parameterTypes = $this->parseTypeSpecifierListOpt();
    $this->eat(TokenKind::CLOSE_PAREN);
    $this->eat(TokenKind::COLON);
    $returnType = $this->parseTypeSpecifier();
    $this->eat(TokenKind::CLOSE_PAREN);

    return new ClosureTypeTree($this->getRange($start), $parameterTypes, $returnType);
  }

  private function parseNullableType(): ParseTree
  {
    $start = $this->position();

    $type = $this->parseTypeSpecifier();

    return new NullableTypeTree($this->getRange($start), $type);
  }

  private function parseKeywordType(): ParseTree
  {
    $start = $this->position();

    $token = $this->next();

    return new KeywordTypeTree($this->getRange($start), $token);
  }

  private function parseReturnType(): ParseTree
  {
    $start = $this->position();
    if ($this->peekPredefinedName(PredefinedName::this)) {
      $token = $this->eatName();
      return new PredefinedNameTypeTree($this->getRange($start), $token);
    }
    return $this->parseTypeSpecifier();
  }

  private function parseTypeArgumentsOpt(): ?ParseTree
  {
    if ($this->peekKind(TokenKind::OPEN_ANGLE)) {
      return $this->parseTypeArguments();
    } else {
      return null;
    }
  }

  private function parseTypeSpecifierListOpt(): ?Vector<ParseTree>
  {
    if ($this->peekTypeSpecifier()) {
      return $this->parseTypeSpecifierList(Vector {});
    } else {
      return null;
    }
  }

  private function parseTypeSpecifierList(Vector<ParseTree> $types): Vector<ParseTree>
  {
    $types[] = $this->parseTypeSpecifier();
    while ($this->eatOpt(TokenKind::COMMA)) {
      $types[] = $this->parseTypeSpecifier();
    }
    return $types;
  }

  private function parseTypeArguments(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::OPEN_ANGLE);
    $types = $this->parseTypeSpecifierList(Vector {});
    $this->eat(TokenKind::CLOSE_ANGLE);

    return new TypeArgumentsTree($this->getRange($start), $types);
  }
}


