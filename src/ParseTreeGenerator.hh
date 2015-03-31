<?hh //strict

require_once 'Char.hh';

class ParseTreeSpecification
{
  public function __construct(
      public string $name,
      public Vector<Pair<string, string>> $fields)
  {}

  public static function CamelToUpper(string $value): string
  {
    $result = $value[0];

    for ($i = 1; $i < strlen($value); $i++) {
      if (Char::isUpperCaseLetter(ord($value[$i]))) {
        $result .= '_';
      }

      $result .= strtoupper($value[$i]);
    }

    return $result;
  }

  public function upperName(): string 
  {
    return ParseTreeSpecification::CamelToUpper($this->name);
  }

  public function typeName(): string
  {
    return $this->name . 'Tree';
  }
}

class ParseTreeSpecifications
{
  public static Vector<ParseTreeSpecification> $specs = Vector {};
}

class ParseTreeGenerator
{

  private static function writeKindFile(string $filename): void
  {

    $file = fopen($filename, 'w');

    fwrite($file, <<< FILE_HEADER
<?hh // strict

enum ParseTreeKind : int
{

FILE_HEADER
);
    $index = 0;
    foreach (ParseTreeSpecifications::$specs as $spec) {
      $name = $spec->upperName();
      fwrite($file, "  $name = $index;\n");
      $index++;
    }
    fwrite($file, "}\n");
    fclose($file);
  }

  private static function writeParseTreeFile(string $filename): void
  {
    $file = fopen($filename, 'w');
    fwrite($file, <<< 'FILE_HEADER'
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

FILE_HEADER
);
    foreach (ParseTreeSpecifications::$specs as $spec) {
      $name = $spec->name;
      $upperName = $spec->upperName();
      $tree = $spec->typeName();
      fwrite($file, <<<IS
  public function is$name(): bool {
    return \$this->kind === ParseTreeKind::$upperName;
  }

IS
);
      fwrite($file, <<<AS
  public function as$name(): $tree {
    invariant(\$this instanceof $tree, "Wrong type.");
    return \$this;
  }

AS
);
    }

    fwrite($file, "}\n");
    fclose($file);
  }

  private static function writeParseTreesFile(string $filename): void
  {
    $file = fopen($filename, 'w');
    fwrite($file, <<< 'FILE_HEADER'
<?hh // strict

require_once 'ParseTree.hh';
require_once 'ParseTreeKind.hh';


FILE_HEADER
);
    foreach (ParseTreeSpecifications::$specs as $spec) {
      $upperName = $spec->upperName();
      $tree = $spec->typeName();
      fwrite($file, <<< CLASS_HEADER

class $tree extends ParseTree
{
  public function __construct(
    Range \$range
CLASS_HEADER
);

      foreach ($spec->fields as $field) {
        $fieldName = $field[0];
        $fieldType = $field[1];
        fwrite($file, ",\n    public $fieldType \$$fieldName");
      }

      fwrite($file, <<< CONSTRUCTOR_BODY
)
  {
    parent::__construct(\$range, ParseTreeKind::$upperName);
  }

CONSTRUCTOR_BODY
);
      // end class
      fwrite($file, "}\n");
    }

    fclose($file);
  }

  private static function initialize(): void 
  {

    ParseTreeSpecifications::$specs = Vector {
      new ParseTreeSpecification("Script",
          Vector {
            Pair { "declarations", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("QualifiedName",
          Vector {
            Pair { "fullyQualified", "bool" },
            Pair { "names", "Vector<NameToken>" }
          }),
      new ParseTreeSpecification("RequireMultipleDirective",
          Vector {
            Pair { "includeFilename", "ParseTree" }
          }),
      new ParseTreeSpecification("RequireOnceDirective",
          Vector {
            Pair { "includeFilename", "ParseTree" }
          }),
      new ParseTreeSpecification("Literal",
          Vector {
            Pair { "value", "Token" }
          }),
      new ParseTreeSpecification("EnumDeclaration",
          Vector {
            Pair { "name", "NameToken" },
            Pair { "base", "ParseTree" },
            Pair { "constraint", "?ParseTree" },
            Pair { "enumerators", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("Enumerator",
          Vector {
            Pair { "name", "NameToken" },
            Pair { "value", "ParseTree" }
          }),
      new ParseTreeSpecification("FunctionDefinition",
          Vector {
            Pair { "isAsync", "bool" },
            Pair { "name", "NameToken" },
            Pair { "typeParameters", "?Vector<ParseTree>" },
            Pair { "parameters", "ParseTree" },
            Pair { "returnType", "ParseTree" },
            Pair { "body", "ParseTree" }
          }),
      new ParseTreeSpecification("ParameterList",
          Vector {
            Pair { "parameters", "?Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("ParameterDeclaration",
          Vector {
            Pair { "type", "ParseTree" },
            Pair { "name", "VariableNameToken" },
            Pair { "defaultValue", "?ParseTree" }
          }),
      new ParseTreeSpecification("CompoundStatement",
          Vector {
            Pair { "statements", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("PredefinedNameType",
          Vector {
            Pair { "token", "NameToken" }
          }),
      new ParseTreeSpecification("KeywordType",
          Vector {
            Pair { "token", "Token" }
          }),
      new ParseTreeSpecification("NullableType",
          Vector {
            Pair { "type", "ParseTree" }
          }),
      new ParseTreeSpecification("ArrayType",
          Vector {
            Pair { "typeParameters", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("NamedType",
          Vector {
            Pair { "name", "ParseTree" },
              Pair { "typeArguments", "?ParseTree" }
          }),
      new ParseTreeSpecification("ClosureType",
          Vector {
            Pair { "parameterTypes", "?Vector<ParseTree>" },
            Pair { "returnType", "ParseTree" }
          }),
      new ParseTreeSpecification("TupleType",
          Vector {
            Pair { "types", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("TypeArguments",
          Vector {
            Pair { "types", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("NamespaceUseDeclaration",
          Vector {
            Pair { "useClauses", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("NamespaceUseClause",
          Vector {
            Pair { "name", "ParseTree" },
            Pair { "alias", "?NameToken" }
          }),
      new ParseTreeSpecification("NamespaceDefinition",
          Vector {
            Pair { "name", "?NameToken" },
            Pair { "declarations", "?Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("GenericTypeParameter",
          Vector {
            Pair { "variance", "?Token" },
            Pair { "name", "NameToken" },
            Pair { "constraint", "?ParseTree" }
          }),
      new ParseTreeSpecification("ClassDeclaration",
          Vector {
            Pair { "isAbstract", "bool" },
            Pair { "isFinal", "bool" },
            Pair { "name", "NameToken" },
            Pair { "typeParameters", "?Vector<ParseTree>" },
            Pair { "extendsClause", "?ParseTree" },
            Pair { "implementsClause", "?Vector<ParseTree>" },
            Pair { "traits", "Vector<ParseTree>" },
            Pair { "members", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("TraitUseClause",
          Vector {
            Pair { "traits", "Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("ConstDeclaration",
          Vector {
            Pair { "type", "?ParseTree" },
            Pair { "declarators", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("ConstDeclarator",
          Vector {
            Pair { "name", "NameToken" },
            Pair { "value", "ParseTree" }
          }),
      new ParseTreeSpecification("PropertyDeclarator",
          Vector {
            Pair { "name", "VariableNameToken" },
            Pair { "initializer", "?ParseTree" }
          }),
      new ParseTreeSpecification("MethodDefinition",
          Vector {
            Pair { "modifiers", "Vector<Token>" },
            Pair { "name", "NameToken" },
            Pair { "typeParameters", "?Vector<ParseTree>" },
            Pair { "parameters", "ParseTree" },
            Pair { "returnType", "ParseTree" },
            Pair { "body", "?ParseTree" }
          }),
      new ParseTreeSpecification("DestructorDeclaration",
          Vector {
            Pair { "modifiers", "Vector<Token>" },
            Pair { "body", "ParseTree" }
          }),
      new ParseTreeSpecification("ConstructorDeclaration",
          Vector {
            Pair { "modifiers", "Vector<Token>" },
            Pair { "parameters", "?Vector<ParseTree>" },
            Pair { "body", "ParseTree" }
          }),
      new ParseTreeSpecification("ConstructorParameter",
          Vector {
            Pair { "modifiers", "Vector<Token>" },
            Pair { "type", "ParseTree" },
            Pair { "name", "VariableNameToken" },
            Pair { "defaultValue", "?ParseTree" }
          }),
      new ParseTreeSpecification("InterfaceDeclaration",
          Vector {
            Pair { "name", "NameToken" },
            Pair { "typeParameters", "?Vector<ParseTree>" },
            Pair { "extendsClause", "?Vector<ParseTree>" },
            Pair { "members", "Vector<ParseTree>" }
          }),
      new ParseTreeSpecification("RequiresExtendsClause",
          Vector {
            Pair { "name", "ParseTree" },
          }),
      new ParseTreeSpecification("RequiresImplementsClause",
          Vector {
            Pair { "name", "ParseTree" },
          }),
      new ParseTreeSpecification("TraitDeclaration",
          Vector {
            Pair { "name", "NameToken" },
            Pair { "typeParameters", "?Vector<ParseTree>" },
            Pair { "traitUseClauses", "Vector<ParseTree>" },
            Pair { "members", "Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("EmptyStatement", Vector {}),
      new ParseTreeSpecification("ExpressionStatement",
          Vector {
            Pair { "expression", "ParseTree" },
          }),
      new ParseTreeSpecification("IfStatement",
          Vector {
            Pair { "condition", "ParseTree" },
            Pair { "thenClause", "ParseTree" },
            Pair { "elseifClauses", "?Vector<ParseTree>" },
            Pair { "elseClause", "?ParseTree" },
          }),
      new ParseTreeSpecification("ElseifClause",
          Vector {
            Pair { "condition", "ParseTree" },
            Pair { "elseClause", "ParseTree" },
          }),
      new ParseTreeSpecification("ElseClause",
          Vector {
            Pair { "elseClause", "ParseTree" },
          }),
      new ParseTreeSpecification("DefaultLabel", Vector {}),
      new ParseTreeSpecification("CaseLabel",
          Vector {
            Pair { "condition", "ParseTree" },
          }),
      new ParseTreeSpecification("CaseClause",
          Vector {
            Pair { "labels", "Vector<ParseTree>" },
            Pair { "statements", "Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("SwitchStatement",
          Vector {
            Pair { "condition", "ParseTree" },
            Pair { "caseClauses", "Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("WhileStatement",
          Vector {
            Pair { "condition", "ParseTree" },
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("DoStatement",
          Vector {
            Pair { "body", "ParseTree" },
            Pair { "condition", "ParseTree" },
          }),
      new ParseTreeSpecification("ForStatement",
          Vector {
            Pair { "initializer", "?Vector<ParseTree>" },
            Pair { "condition", "Vector<ParseTree>" },
            Pair { "increment", "?Vector<ParseTree>" },
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("AliasExpression",
          Vector {
            Pair { "expression", "ParseTree" },
          }),
      new ParseTreeSpecification("ForEachStatement",
          Vector {
            Pair { "collection", "ParseTree" },
            Pair { "key", "?ParseTree" },
            Pair { "value", "ParseTree" },
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("ContinueStatement", Vector {}),
      new ParseTreeSpecification("BreakStatement", Vector {}),
      new ParseTreeSpecification("ReturnStatement",
          Vector {
            Pair { "value", "?ParseTree" },
          }),
      new ParseTreeSpecification("ThrowStatement",
          Vector {
            Pair { "value", "ParseTree" },
          }),
      new ParseTreeSpecification("TryStatement",
          Vector {
            Pair { "body", "ParseTree" },
            Pair { "catchClauses", "Vector<ParseTree>" },
            Pair { "finallyClause", "?ParseTree" },
          }),
      new ParseTreeSpecification("CatchClause",
          Vector {
            Pair { "type", "ParseTree" },
            Pair { "name", "VariableNameToken" },
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("FinallyClause",
          Vector {
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("YieldExpression",
          Vector {
            Pair { "value", "ParseTree" },
          }),
      new ParseTreeSpecification("ArrayElementInitializer",
          Vector {
            Pair { "key", "ParseTree" },
            Pair { "value", "ParseTree" },
          }),
      new ParseTreeSpecification("CollectionLiteral",
          Vector {
            Pair { "name", "ParseTree" },
            Pair { "elements", "?Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("VariableName",
          Vector {
            Pair { "name", "VariableNameToken" },
          }),
      new ParseTreeSpecification("ShapeLiteral",
          Vector {
            Pair { "initializers", "?Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("TupleLiteral",
          Vector {
            Pair { "values", "Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("FieldInitializer",
          Vector {
            Pair { "name", "ParseTree" },
            Pair { "value", "ParseTree" },
          }),
      new ParseTreeSpecification("AnonymousFunction",
          Vector {
            Pair { "isAsync", "bool" },
            Pair { "parameters", "?Vector<ParseTree>" },
            Pair { "returnType", "?ParseTree" },
            Pair { "useClause", "?Vector<ParseTree>" },
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("AnonymousFunctionParameter",
          Vector {
            Pair { "type", "?ParseTree" },
            Pair { "name", "VariableNameToken" },
            Pair { "defaultValue", "?ParseTree" },
          }),
      new ParseTreeSpecification("ObjectCreationExpression",
          Vector {
            Pair { "type", "?ParseTree" },
            Pair { "arguments", "?Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("ArrayLiteral",
          Vector {
            Pair { "elements", "?Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("SubscriptOperator",
          Vector {
            Pair { "collection", "ParseTree" },
            Pair { "index", "?ParseTree" },
          }),
      new ParseTreeSpecification("FunctionCall",
          Vector {
            Pair { "function", "ParseTree" },
            Pair { "arguments", "?Vector<ParseTree>" },
          }),
      new ParseTreeSpecification("MemberSelection",
          Vector {
            Pair { "object", "ParseTree" },
            Pair { "name", "NameToken" },
          }),
      new ParseTreeSpecification("NullSafeMemberSelection",
          Vector {
            Pair { "object", "ParseTree" },
            Pair { "name", "NameToken" },
          }),
      new ParseTreeSpecification("PostfixOperator",
          Vector {
            Pair { "value", "ParseTree" },
            Pair { "operator", "Token" },
          }),
      new ParseTreeSpecification("ScopeResolution",
          Vector {
            Pair { "baseName", "ParseTree" },
            Pair { "memberName", "Token" },
          }),
      new ParseTreeSpecification("StaticName",
          Vector {
            Pair { "static", "Token" },
          }),
      new ParseTreeSpecification("BinaryExpression",
          Vector {
            Pair { "left", "ParseTree" },
            Pair { "operator", "Token" },
            Pair { "right", "ParseTree" },
          }),
      new ParseTreeSpecification("UnaryExpression",
          Vector {
            Pair { "operator", "Token" },
            Pair { "value", "ParseTree" },
          }),
      new ParseTreeSpecification("CastExpression",
          Vector {
            Pair { "type", "ParseTree" },
            Pair { "value", "ParseTree" },
          }),
      new ParseTreeSpecification("ConditionalExpression",
          Vector {
            Pair { "condition", "ParseTree" },
            Pair { "trueVaule", "?ParseTree" },
            Pair { "falseValue", "ParseTree" },
          }),
      new ParseTreeSpecification("LambdaExpression",
          Vector {
            Pair { "isAsync", "bool" },
            Pair { "signature", "ParseTree" },
            Pair { "body", "ParseTree" },
          }),
      new ParseTreeSpecification("LambdaSignature",
          Vector {
            Pair { "parameters", "?Vector<ParseTree>" },
            Pair { "returnType", "?ParseTree" },
          }),
      new ParseTreeSpecification("ParenExpression",
          Vector {
            Pair { "expression", "ParseTree" },
          }),
      new ParseTreeSpecification("ParseError", Vector {}),
    };
  }

  public static function main(array<string> $argv): int
  {
    $kindFileName = $argv[1];
    $parseTreeFileName = $argv[2];
    $parseTreesFileName = $argv[3];

    ParseTreeGenerator::initialize();
    ParseTreeGenerator::writeKindFile($kindFileName);
    ParseTreeGenerator::writeParseTreeFile($parseTreeFileName);
    ParseTreeGenerator::writeParseTreesFile($parseTreesFileName);
    return 0;
  }
}
