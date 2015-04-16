<?hh // strict

namespace Syntax {

require_once 'ParseTrees.hh';

class Trees {
  public static function lastNameOf(QualifiedNameTree $tree): string
  {
    return $tree->names[$tree->names->count() - 1]->value();
  }

  public static function nameOfNamespaceUseClause(
    NamespaceUseClauseTree $tree): string
  {
    if ($tree->alias !== null) {
      return $tree->alias->value();
    } else {
      return Trees::lastNameOf($tree->name->asQualifiedName());
    }
  }

  public static function ctorOfClassDeclaration(
    ClassDeclarationTree $tree): ?ConstructorDeclarationTree
  {
    foreach ($tree->members as $member) {
      if ($member instanceof ConstructorDeclarationTree) {
        return $member;
      }
    }
    return null;
  }

  public static function isSelf(ParseTree $tree): bool
  {
    return self::isName($tree, PredefinedName::self);
  }

  public static function isParent(ParseTree $tree): bool
  {
    return self::isName($tree, PredefinedName::parent);
  }

  public static function isName(ParseTree $tree, string $value): bool
  {
    if (!$tree->isQualifiedName() || $tree->asQualifiedName()->fullyQualified) {
      return false;
    }
    $names = $tree->asQualifiedName()->names;
    return $names->count() === 1 && $names[0]->text() === $value;
  }
}

}


