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
    case TokenKind::KW_INTERFACE:
    case TokenKind::KW_TRAIT:
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
    case TokenKind::KW_INTERFACE:
      return $this->parseInterfaceDeclaration();
    case TokenKind::KW_TRAIT:
      return $this->parseTraitDeclaration();
    default:
      // TODO
      throw new Exception();
    }
  }

  private function parseTraitDeclaration(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_TRAIT);
    $name = $this->eatName();
    $typeParameters = $this->parseTypeParametersOpt();
    $this->eat(TokenKind::OPEN_CURLY);
    $traitUseClauses = $this->parseList(
      () ==> $this->peekTraitUseClause(),
      () ==> $this->parseTraitUseClause());
    $members = $this->parseList(
      () ==> $this->peekTraitMember(),
      () ==> $this->parseTraitMember());
    $this->eat(TokenKind::CLOSE_CURLY);

    return new TraitDeclarationTree(
      $this->getRange($start),
      $name,
      $typeParameters,
      $traitUseClauses,
      $members);
  }

  private function peekTraitMember(): bool
  {
    switch ($this->peek()) {
    case TokenKind::KW_REQUIRE:
    case TokenKind::LEFT_SHIFT:
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

  private function parseTraitMember(): ParseTree
  {
    switch ($this->peek()) {
    case TokenKind::KW_REQUIRE:
      switch ($this->peek()) {
      case TokenKind::KW_EXTENDS:
        return $this->parseRequireExtendsClause();
      case TokenKind::KW_IMPLEMENTS:
        return $this->parseRequireImplementsClause();
      default:
        $this->error("'extends' or 'implements' expected.");
        return new ParseErrorTree($this->getRange($this->position()));
      }
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
      throw new Exception();
    }
  }

  private function parseInterfaceDeclaration(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_INTERFACE);
    $name = $this->eatName();
    $typeParameters = $this->parseTypeParametersOpt();
    $extendsClause = $this->parseExtendsClause();
    $members = $this->parseDelimitedList(
      TokenKind::OPEN_CURLY,
      TokenKind::CLOSE_CURLY,
      () ==> $this->peekInterfaceMember(),
      () ==> $this->parseInterfaceMember());

    return new InterfaceDeclarationTree(
      $this->getRange($start),
      $name,
      $typeParameters,
      $extendsClause,
      $members);
  }

  private function parseExtendsClause(): ?Vector<ParseTree>
  {
    if ($this->eatOpt(TokenKind::KW_EXTENDS)) {
      return 
        $this->parseCommaSeparatedList(() ==> $this->parseQualifiedNameType());
    } else {
      return null;
    }
  }

  private function peekInterfaceMember(): bool
  {
    switch ($this->peek()) {
    case TokenKind::KW_CONST:
    case TokenKind::LEFT_SHIFT:
    case TokenKind::KW_PUBLIC:
    case TokenKind::KW_PROTECTED:
    case TokenKind::KW_PRIVATE:
    case TokenKind::KW_STATIC:
    case TokenKind::KW_ABSTRACT:
    case TokenKind::KW_FINAL:
    case TokenKind::KW_REQUIRE:
      return true;
    default:
      return false;
    }
  }

  private function parseInterfaceMember(): ParseTree
  {
    switch ($this->peek()) {
    case TokenKind::KW_CONST:
      return $this->parseConstDeclaration();
    case TokenKind::KW_REQUIRE:
      return $this->parseRequireExtendsClause();
    case TokenKind::LEFT_SHIFT:
      // TODO: attributes
    case TokenKind::KW_PUBLIC:
    case TokenKind::KW_PROTECTED:
    case TokenKind::KW_PRIVATE:
    case TokenKind::KW_STATIC:
    case TokenKind::KW_ABSTRACT:
    case TokenKind::KW_FINAL:
      $modifiers = $this->parseModifiers();
      // TODO: no constructors or destructors
      return $this->parseMethodLikeDeclaration($modifiers);

    default:
      throw new Exception("");
    }
  }

  private function parseRequireImplementsClause(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_REQUIRE);
    $this->eat(TokenKind::KW_IMPLEMENTS);
    $name = $this->parseQualifiedName();

    return new RequiresImplementsClauseTree($this->getRange($start), $name);
  }

  private function parseRequireExtendsClause(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_REQUIRE);
    $this->eat(TokenKind::KW_EXTENDS);
    $name = $this->parseQualifiedName();

    return new RequiresExtendsClauseTree($this->getRange($start), $name);
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
    $value = $this->parseInitializer();
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
        () ==> $this->peekTraitUseClause(),
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
    switch ($this->peek()) {
    case TokenKind::KW_CONST:
    case TokenKind::LEFT_SHIFT:
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
    $initializer = $this->parseInitializerOpt();

    return new PropertyDeclaratorTree(
      $this->getRange($start),
      $name,
      $initializer);
  }

  private function parseInitializer(): ParseTree
  {
    $this->eat(TokenKind::EQUAL);
    return $this->parseExpression();
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
    $value = $this->parseInitializer();

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

  private function peekTraitUseClause(): bool
  {
    return $this->peekKind(TokenKind::KW_USE);
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

  private function peekExpression(): bool
  {
    // TODO
    return false;
  }

  private function parseExpressionOpt(): ?ParseTree
  {
    if ($this->peekExpression()) {
      return $this->parseExpression();
    } else {
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

  // Statements
  private function peekStatement(): bool
  {
    // TODO
    return false;
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

  private function parseEmptyStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::SEMI_COLON);

    return new EmptyStatementTree($this->getRange($start));
  }

  private function parseExpressionStatement(): ParseTree
  {
    $start = $this->position();

    $expression = $this->parseExpression();
    $this->eat(TokenKind::SEMI_COLON);

    return new ExpressionStatementTree(
      $this->getRange($start),
      $expression);
  }

  private function parseIfStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_IF);
    $condition = $this->parseParenExpression();
    $thenClause = $this->parseStatement();
    $elseifClauses = $this->parseList(
      () ==> $this->peekKind(TokenKind::KW_ELSEIF),
      () ==> $this->parseElseifClause());
    $elseClause = $this->parseElseClauseOpt();

    return new IfStatementTree(
      $this->getRange($start),
      $condition,
      $thenClause,
      $elseifClauses,
      $elseClause);
  }

  private function parseParenExpression(): ParseTree
  {
    $this->eat(TokenKind::OPEN_PAREN);
    $result = $this->parseExpression();
    $this->eat(TokenKind::CLOSE_PAREN);

    return $result;
  }

  private function parseElseifClause(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_ELSEIF);
    $condition = $this->parseParenExpression();
    $thenClause = $this->parseStatement();

    return new ElseifClauseTree(
      $this->getRange($start),
      $condition,
      $thenClause);
  }

  private function parseElseClauseOpt(): ?ParseTree
  {
    if ($this->peekKind(TokenKind::KW_ELSE)) {
      $start = $this->position();

      $this->eat(TokenKind::KW_ELSE);
      $elseClause = $this->parseStatement();

      return new ElseClauseTree(
        $this->getRange($start),
        $elseClause);
    } else {
      return null;
    }
  }

  private function parseSwitchStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_SWITCH);
    $condition = $this->parseParenExpression();
    $this->eat(TokenKind::OPEN_CURLY);
    $caseClauses = $this->parseList(
      () ==> $this->peekCaseClause(),
      () ==> $this->parseCaseClause());
    $this->eat(TokenKind::CLOSE_CURLY);

    return new SwitchStatementTree(
      $this->getRange($start),
      $condition,
      $caseClauses);
  }

  private function peekCaseClause(): bool
  {
    switch ($this->peek()) {
    case TokenKind::KW_CASE:
    case TokenKind::KW_DEFAULT:
      return true;
    default:
      return false;
    }
  }

  private function parseCaseClause(): ParseTree
  {
    $start = $this->position();

    $labels = $this->parseList(
      () ==> $this->peekCaseClause(),
      () ==> $this->parseCaseLabel());
    $statement = $this->parseStatement();

    return new CaseClauseTree(
      $this->getRange($start),
      $labels,
      $statement);
  }

  private function parseWhileStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_WHILE);
    $condition = $this->parseParenExpression();
    $body = $this->parseStatement();

    return new WhileStatementTree(
      $this->getRange($start),
      $condition,
      $body);
  }

  private function parseDoStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_DO);
    $body = $this->parseStatement();
    $this->eat(TokenKind::KW_WHILE);
    $condition = $this->parseParenExpression();
    $this->eat(TokenKind::SEMI_COLON);

    return new WhileStatementTree(
      $this->getRange($start),
      $condition,
      $body);
  }

  private function parseForStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_FOR);
    $this->eat(TokenKind::OPEN_PAREN);
    $initializer = $this->parseForExpressionGroup();
    $this->eat(TokenKind::SEMI_COLON);
    $condition = $this->parseForExpressionGroup();
    if ($condition === null) {
      $this->error("Expected expression.");
      $condition = Vector {};
    }
    $this->eat(TokenKind::SEMI_COLON);
    $increment = $this->parseForExpressionGroup();
    $this->eat(TokenKind::CLOSE_PAREN);
    $body = $this->parseStatement();

    return new ForStatementTree(
      $this->getRange($start),
      $initializer,
      $condition,
      $increment,
      $body);
  }

  private function parseForEachStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_FOREACH);
    $this->eat(TokenKind::OPEN_PAREN);
    $collection = $this->parseExpression();
    $this->eat(TokenKind::KW_AS);
    if ($this->peekKind(TokenKind::AMPERSAND)) {
      $key = null;
      $value = $this->parseAliasExpression();
    } else {
      $expression = $this->parseExpression();
      if ($this->eatOpt(TokenKind::EQUAL)) {
        $this->eat(TokenKind::CLOSE_ANGLE);
        $key = $expression;
        $value = $this->parseAliasExpression();
      } else {
        $key = null;
        $value = $expression;
      }
    }
    $this->eat(TokenKind::CLOSE_PAREN);
    $body = $this->parseStatement();

    return new ForEachStatementTree(
      $this->getRange($start),
      $collection,
      $key,
      $value,
      $body);
  }

  private function parseContinueStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_CONTINUE);
    $this->eat(TokenKind::SEMI_COLON);

    return new ContinueStatementTree($this->getRange($start));
  }

  private function parseBreakStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_BREAK);
    $this->eat(TokenKind::SEMI_COLON);

    return new BreakStatementTree($this->getRange($start));
  }

  private function parseReturnStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_RETURN);
    $value = $this->parseExpressionOpt();
    $this->eat(TokenKind::SEMI_COLON);

    return new ReturnStatementTree($this->getRange($start), $value);
  }

  private function parseThrowStatement(): ParseTree
  {
    $start = $this->position();

    $this->eat(TokenKind::KW_THROW);
    $value = $this->parseExpression();
    $this->eat(TokenKind::SEMI_COLON);

    return new ThrowStatementTree($this->getRange($start), $value);
  }

  private function parseAliasExpression(): ParseTree
  {
    $start = $this->position();

    if ($this->eatOpt(TokenKind::AMPERSAND)) {
      $expression = $this->parseExpression();
      return new AliasExpressionTree(
        $this->getRange($start),
        $expression);
    }

    return $this->parseExpression();
  }

  private function parseForExpressionGroup(): ?Vector<ParseTree>
  {
    return $this->parseCommaSeparatedListOpt(
      () ==> $this->peekExpression(),
      () ==> $this->parseExpression());
  }

  private function parseCaseLabel(): ParseTree
  {
    $start = $this->position();

    if ($this->eatOpt(TokenKind::KW_DEFAULT)) {
      $this->eat(TokenKind::COLON);
      return new DefaultLabelTree(
        $this->getRange($start));
    } else {
      $this->eat(TokenKind::KW_CASE);
      $condition = $this->parseExpression();
      return new CaseLabelTree(
        $this->getRange($start),
        $condition);
    }
  }

  // List helpers
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

