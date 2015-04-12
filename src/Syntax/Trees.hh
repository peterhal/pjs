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

}

}


