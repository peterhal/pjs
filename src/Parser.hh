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

    $declarations = $this->parseList(
      () ==> $this->peekDeclaration(),
      () ==> $this->parseDeclaration());
    if (!$this->eof()) {
      $this->error("Expected declaration.");
    }

    return new ScriptTree(
        $this->getRange($start),
        $declarations);
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

    $useClauses = 
      $this->parseDelimitedCommaSeparatedList(
        TokenKind::KW_USE,
        TokenKind::SEMI_COLON,
        () ==> $this->parseNamespaceUseClause());

    return new NamespaceUseDeclarationTree(
      $this->getRange($start),
      $useClauses);
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
        $declarations = $this->parseDelimitedList(
          TokenKind::OPEN_CURLY,
          TokenKind::CLOSE_CURLY,
          () ==> $this->peekDeclaration(),
          () ==> $this->parseDeclaration());

        return new NamespaceDefinitionTree(
          $this->getRange($start),
          $name,
          $declarations);
      }
    } else {
        $declarations = $this->parseDelimitedList(
          TokenKind::OPEN_CURLY,
          TokenKind::CLOSE_CURLY,
          () ==> $this->peekDeclaration(),
          () ==> $this->parseDeclaration());

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

    $enumerators = $this->parseDelimitedList(
      TokenKind::OPEN_CURLY,
      TokenKind::CLOSE_CURLY,
      () ==> $this->peekEnumerator(),
      () ==> $this->parseEnumerator());
    if ($enumerators->count() < 1) {
      $this->error("Expectd enumerator.");
    }

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

  private function peekEnumerator(): bool
  {
    return $this->peekKind(TokenKind::NAME);
  }

  private function parseEnumerator(): ParseTree
  {
    $start = $this->position();

    $name = $this->eatName();
    $this->eat(TokenKind::EQUAL);
    $value = $this->parseExpression();
    $this->eat(TokenKind::SEMI_COLON);

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
      $implementsClause =
        $this->parseCommaSeparatedList(() ==> $this->parseQualifiedNameType());
    } else {
      $implementsClause = null;
    }
    $traits = $this->parseList(
        () ==> $this->peekKind(TokenKind::KW_USE),
        () ==> $this->parseTraitUseClause());
    $members = $this->parseList(
        () ==> $this->peekClassMember(),
        () ==> $this->parseClassMember());

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

  private function peekClassMember(): bool
  {
    // TODO:
    return false;
  }

  private function parseClassMember(): ParseTree
  {
    switch ($this->peek()) {
    case TokenKind::KW_CONST:
      return $this->parseConstDeclaration();
    case TokenKind::LEFT_SHIFT:
      // TODO: attributes
    case TokenKind::KW_PUBLIC:
    case TokenKind::KW_PROTECTED:
    case TokenKind::KW_PRIVATE:
    case TokenKind::KW_STATIC:
    case TokenKind::KW_ABSTRACT:
    case TokenKind::KW_FINAL:
      $modifiers = $this->parseModifiers();
      switch ($this->peek()) {
      case TokenKind::KW_FUNCTION:
        return $this->parseMethodLikeDeclaration($modifiers);
      case TokenKind::VARIABLE_NAME:
        return $this->parsePropertyDeclaration($modifiers);
      default:
        $this->error("Class member expected");
        return new ParseErrorTree($this->getRange($this->position()));
      }

    default:
      throw new Exception("");
    }
  }

  private function parseConstructorDeclaration(
    Vector<Token> $modifiers,
    NameToken $name): ParseTree
  {
    $start = $modifiers[0]->start();

    $this->eat(TokenKind::OPEN_PAREN);
    $parameters = $this->parseCommaSeparatedListOpt(
      () ==> $this->peekConstructorParameter(),
      () ==> $this->parseConstructorParameter());
    $this->eat(TokenKind::CLOSE_PAREN);
    $body = $this->parseCompoundStatement();

    return new ConstructorDeclarationTree(
      $this->getRange($start),
      $modifiers,
      $parameters,
      $body);
  }

  private function peekConstructorParameter(): bool
  {
    return $this->peek() || Parser::isModifier($this->peek());
  }

  private function parseConstructorParameter(): ParseTree
  {
    $start = $this->position();

    $modifiers = $this->parseModifiers();
    $type = $this->parseTypeSpecifier();
    $name = $this->eatVariableName();
    $defaultValue = $this->parseInitializerOpt();

    return new ConstructorParameterTree(
      $this->getRange($start),
      $modifiers,
      $type,
      $name,
      $defaultValue);
  }

  private function parseDestructorDeclaration(
    Vector<Token> $modifiers,
    NameToken $name): ParseTree
  {
    $start = $modifiers[0]->start();

    $this->eat(TokenKind::OPEN_PAREN);
    $this->eat(TokenKind::CLOSE_PAREN);
    $body = $this->parseCompoundStatement();

    return new DestructorDeclarationTree(
      $this->getRange($start),
      $modifiers,
      $body);
  }

  private function parseMethodDeclaration(
    Vector<Token> $modifiers,
    NameToken $name): ParseTree
  {
    $start = $modifiers[0]->start();

    // TODO: type parameters
    $parameters = $this->parseParameterList();
    $this->eat(TokenKind::COLON);
    $returnType = $this->parseReturnType();
    $body = $this->parseCompoundStatement();

    return new MethodDefinitionTree(
      $this->getRange($start),
      $modifiers,
      $name,
      $parameters,
      $returnType,
      $body);
  }

  private function parseMethodLikeDeclaration(Vector<Token> $modifiers): ParseTree
  {
    $this->eat(TokenKind::KW_FUNCTION);
    $name = $this->eatName();
    if ($name === PredefinedName::__construct) {
      return $this->parseConstructorDeclaration(
        $modifiers,
        $name);
    } else if ($name === PredefinedName::__destruct) {
      return $this->parseDestructorDeclaration(
        $modifiers,
        $name);
    } else {
      return $this->parseMethodDeclaration(
        $modifiers,
        $name);
    }
  }

  private function parsePropertyDeclaration(Vector<Token> $modifiers): ParseTree
  {
    $start = $modifiers[0]->start();

    $type = $this->parseTypeSpecifier();
    $declarators = $this->parseCommaSeparatedList(
      () ==> $this->parsePropertyDeclarator());
    $this->eat(TokenKind::SEMI_COLON);

    return new ParseErrorTree($this->getRange($this->position()));
  }

  private function parsePropertyDeclarator(): ParseTree
  {
    $start = $this->position();

    $name = $this->eatVariableName();
    if ($this->eatOpt(TokenKind::EQUAL)) {
      $initializer = $this->parseExpression();
    } else {
      $initializer = null;
    }

    return new PropertyDeclaratorTree(
      $this->getRange($start),
      $name,
      $initializer);
  }

  private function parseInitializerOpt(): ?ParseTree
  {
    if ($this->eatOpt(TokenKind::EQUAL)) {
      return $this->parseExpression();
    } else {
      return null;
    }
  }

  private function parseConstDeclaration(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_CONST);
    if (!$this->peekKind(TokenKind::NAME) 
        || !$this->peekIndexKind(1, TokenKind::EQUAL)) {
      $type = $this->parseTypeSpecifier();
    } else {
      $type = null;
    }
    $declarators = $this->parseCommaSeparatedList(
      () ==> $this->parseConstDeclarator());

    return new ConstDeclarationTree(
      $this->getRange($start),
      $type,
      $declarators);
  }

  private function parseConstDeclarator(): ParseTree
  {
    $start = $this->position();

    $name = $this->eatName();
    $this->eat(TokenKind::EQUAL);
    $value = $this->parseExpression();

    return new ConstDeclaratorTree(
      $this->getRange($start),
      $name,
      $value);
  }

  private function parseModifiers(): Vector<Token>
  {
    $result = Vector {};
    while (Parser::isModifier($this->peek())) {
      $result[] = $this->next();
    }
    return $result;
  }

  private static function isModifier(TokenKind $kind): bool
  {
    switch ($kind) {
    case TokenKind::KW_PUBLIC:
    case TokenKind::KW_PROTECTED:
    case TokenKind::KW_PRIVATE:
    case TokenKind::KW_STATIC:
    case TokenKind::KW_ABSTRACT:
    case TokenKind::KW_FINAL:
      return true;
    default:
      return false;
    }
  }

  private function parseTraitUseClause(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_USE);

    $traits = 
      $this->parseCommaSeparatedList(() ==> $this->parseQualifiedNameType());

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

      $result = $this->parseDelimitedCommaSeparatedList(
        TokenKind::OPEN_ANGLE,
        TokenKind::CLOSE_ANGLE,
        () ==> $this->parseTypeParameter());

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

    $parameters = $this->parseCommaSeparatedListOpt(
        () ==> $this->peekParameter(),
        () ==> $this->parseParameterDeclaration());

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

    $statements = $this->parseDelimitedList(
      TokenKind::OPEN_CURLY,
      TokenKind::CLOSE_CURLY,
      () ==> $this->peekStatement(),
      () ==> $this->parseStatement());

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

    $types =
      $this->parseDelimitedCommaSeparatedList(
        TokenKind::OPEN_PAREN,
        TokenKind::CLOSE_PAREN,
        () ==> $this->parseTypeSpecifier());
    if ($types->count() < 2) {
      $this->error("Tuple type requires at least 2 types.");
    }

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
    return $this->parseCommaSeparatedListOpt(
      () ==> $this->peekTypeSpecifier(),
      () ==> $this->parseTypeSpecifier());
  }

  private function parseTypeArguments(): ParseTree
  {
    $start = $this->position();

    $types =
      $this->parseDelimitedCommaSeparatedList(
        TokenKind::OPEN_ANGLE,
        TokenKind::CLOSE_ANGLE,
        () ==> $this->parseTypeSpecifier());

    return new TypeArgumentsTree($this->getRange($start), $types);
  }

  private function parseCommaSeparatedListOpt(
    (function (): bool) $peek,
    (function (): ParseTree) $element): ?Vector<ParseTree>
  {
    if ($peek()) {
      return $this->parseCommaSeparatedList($element);
    } else {
      return null;
    }
  }

  private function parseDelimitedCommaSeparatedList(
    TokenKind $first,
    TokenKind $end,
    (function (): ParseTree) $element): Vector<ParseTree>
  {
    $this->eat($first);
    $result = $this->parseCommaSeparatedList($element);
    $this->eat($end);
    return $result;
  }

  private function parseCommaSeparatedList(
    (function (): ParseTree) $element): Vector<ParseTree>
  {
    return $this->parseSeparatedList(TokenKind::COMMA, $element);
  }

  private function parseSeparatedList(
    TokenKind $separator,
    (function (): ParseTree) $element): Vector<ParseTree>
  {
    $result = Vector {};
    $result[] = $element();
    while ($this->eatOpt($separator)) {
      $result[] = $element();
    }
    return $result;
  }

  private function parseDelimitedList(
    TokenKind $first,
    TokenKind $end,
    (function(): bool) $peek,
    (function(): ParseTree) $element): Vector<ParseTree>
  {
    $this->eat($first);
    $result = $this->parseList($peek, $element);
    $this->eat($end);
    return $result;
  }

  private function parseList(
    (function(): bool) $peek,
    (function(): ParseTree) $element): Vector<ParseTree>
  {
    $result = Vector {};
    while ($peek()) {
      $result[] = $element();
    }
    return $result;
  }
}

