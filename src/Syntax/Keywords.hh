<?hh // strict

namespace Syntax {

require_once 'TokenKind.hh';
require_once 'Utils/utils.hh';

class Keywords
{
  const string KW_ABSTRACT = 'abstract';
  const string KW_ARRAY = 'array';
  const string KW_ARRAYKEY = 'arraykey';
  const string KW_AS = 'as';
  const string KW_ASYNC = 'async';
  const string KW_AWAIT = 'await';
  const string KW_BREAK = 'break';
  const string KW_CASE = 'case';
  const string KW_CATCH = 'catch';
  const string KW_CLASS = 'class';
  const string KW_CLONE = 'clone';
  const string KW_CONST = 'const';
  const string KW_CONTINUE = 'continue';
  const string KW_DEFAULT = 'default';
  const string KW_DIE = 'die';
  const string KW_DO = 'do';
  const string KW_ECHO = 'echo';
  const string KW_ELSE = 'else';
  const string KW_ELSEIF = 'elseif';
  const string KW_ENUM = 'enum';
  const string KW_EXIT = 'exit';
  const string KW_EXTENDS = 'extends';
  const string KW_FINAL = 'final';
  const string KW_FINALLY = 'finally';
  const string KW_FOR = 'for';
  const string KW_FOREACH = 'foreach';
  const string KW_FUNCTION = 'function';
  const string KW_IF = 'if';
  const string KW_IMPLEMENTS = 'implements';
  const string KW_INSTANCEOF = 'instanceof';
  const string KW_INSTEADOF = 'insteadof';
  const string KW_INTERFACE = 'interface';
  const string KW_LIST = 'list';
  const string KW_NAMESPACE = 'namespace';
  const string KW_NEW = 'new';
  const string KW_NEWTYPE = 'newtype';
  const string KW_NUM = 'num';
  const string KW_PRIVATE = 'private';
  const string KW_PROTECTED = 'protected';
  const string KW_PUBLIC = 'public';
  const string KW_REQUIRE = 'require';
  const string KW_REQUIRE_ONCE = 'require_once';
  const string KW_RETURN = 'return';
  const string KW_SHAPE = 'shape';
  const string KW_STATIC = 'static';
  const string KW_SWITCH = 'switch';
  const string KW_THROW = 'throw';
  const string KW_TRAIT = 'trait';
  const string KW_TRY = 'try';
  const string KW_TUPLE = 'tuple';
  const string KW_TYPE = 'type';
  const string KW_USE = 'use';
  const string KW_WHILE = 'while';
  const string KW_YIELD = 'yield';

  // TODO: legacy keywords:
  // endif
  // endwhile
  // endfor
  // endforeach
  // declare
  // enddeclare
  // endswitch
  // goto
  // print
  // callable
  // eval
  // include
  // include_once
  // global
  // isset
  // empty
  // __halt_compiler
  // __copmiler_halt_offset__
  // unset
  // OR
  // AND
  // XOR
  // var

  // TODO: query operators
  // from
  // where
  // join
  // in
  // on
  // equals
  // into
  // let
  // orderby
  // ascending
  // descending
  // select
  // group
  // by

  // TODO: Macro like
  // __CLASS__
  // __TRAIT__
  // __FUNCTION__
  // __METHOD__
  // __LINE__
  // __FILE__
  // __DIR__
  // __NAMESPACE__

  const string KW_FALSE = 'false';
  const string KW_NULL = 'null';
  const string KW_TRUE = 'true';

  private static Map<string, TokenKind> $all = Map {
    Keywords::KW_ABSTRACT => TokenKind::KW_ABSTRACT,
    Keywords::KW_ARRAY => TokenKind::KW_ARRAY,
    Keywords::KW_ARRAYKEY => TokenKind::KW_ARRAYKEY,
    Keywords::KW_AS => TokenKind::KW_AS,
    Keywords::KW_ASYNC => TokenKind::KW_ASYNC,
    Keywords::KW_AWAIT => TokenKind::KW_AWAIT,
    Keywords::KW_BREAK => TokenKind::KW_BREAK,
    Keywords::KW_CASE => TokenKind::KW_CASE,
    Keywords::KW_CATCH => TokenKind::KW_CATCH,
    Keywords::KW_CLASS => TokenKind::KW_CLASS,
    Keywords::KW_CLONE => TokenKind::KW_CLONE,
    Keywords::KW_CONST => TokenKind::KW_CONST,
    Keywords::KW_CONTINUE => TokenKind::KW_CONTINUE,
    Keywords::KW_DEFAULT => TokenKind::KW_DEFAULT,
    Keywords::KW_DIE => TokenKind::KW_DIE,
    Keywords::KW_DO => TokenKind::KW_DO,
    Keywords::KW_ECHO => TokenKind::KW_ECHO,
    Keywords::KW_ELSE => TokenKind::KW_ELSE,
    Keywords::KW_ELSEIF => TokenKind::KW_ELSEIF,
    Keywords::KW_ENUM => TokenKind::KW_ENUM,
    Keywords::KW_EXTENDS => TokenKind::KW_EXTENDS,
    Keywords::KW_EXIT => TokenKind::KW_EXIT,
    Keywords::KW_FINAL => TokenKind::KW_FINAL,
    Keywords::KW_FINALLY => TokenKind::KW_FINALLY,
    Keywords::KW_FOR => TokenKind::KW_FOR,
    Keywords::KW_FOREACH => TokenKind::KW_FOREACH,
    Keywords::KW_FUNCTION => TokenKind::KW_FUNCTION,
    Keywords::KW_IF => TokenKind::KW_IF,
    Keywords::KW_IMPLEMENTS => TokenKind::KW_IMPLEMENTS,
    Keywords::KW_INSTANCEOF => TokenKind::KW_INSTANCEOF,
    Keywords::KW_INSTEADOF => TokenKind::KW_INSTEADOF,
    Keywords::KW_INTERFACE => TokenKind::KW_INTERFACE,
    Keywords::KW_LIST => TokenKind::KW_LIST,
    Keywords::KW_NAMESPACE => TokenKind::KW_NAMESPACE,
    Keywords::KW_NEW => TokenKind::KW_NEW,
    Keywords::KW_NEWTYPE => TokenKind::KW_NEWTYPE,
    Keywords::KW_NUM => TokenKind::KW_NUM,
    Keywords::KW_PRIVATE => TokenKind::KW_PRIVATE,
    Keywords::KW_PROTECTED => TokenKind::KW_PROTECTED,
    Keywords::KW_PUBLIC => TokenKind::KW_PUBLIC,
    Keywords::KW_REQUIRE => TokenKind::KW_REQUIRE,
    Keywords::KW_REQUIRE_ONCE => TokenKind::KW_REQUIRE_ONCE,
    Keywords::KW_RETURN => TokenKind::KW_RETURN,
    Keywords::KW_SHAPE => TokenKind::KW_SHAPE,
    Keywords::KW_STATIC => TokenKind::KW_STATIC,
    Keywords::KW_SWITCH => TokenKind::KW_SWITCH,
    Keywords::KW_THROW => TokenKind::KW_THROW,
    Keywords::KW_TRAIT => TokenKind::KW_TRAIT,
    Keywords::KW_TRY => TokenKind::KW_TRY,
    Keywords::KW_TUPLE => TokenKind::KW_TUPLE,
    Keywords::KW_TYPE => TokenKind::KW_TYPE,
    Keywords::KW_USE => TokenKind::KW_USE,
    Keywords::KW_WHILE => TokenKind::KW_WHILE,
    Keywords::KW_YIELD => TokenKind::KW_YIELD,

    Keywords::KW_FALSE => TokenKind::KW_FALSE,
    Keywords::KW_NULL => TokenKind::KW_NULL,
    Keywords::KW_TRUE => TokenKind::KW_TRUE
  };

  public static function isKeyword(string $value): bool
  {
    return Keywords::$all->containsKey($value);
  }

  public static function getKeyword(string $value): TokenKind
  {
    return \Utils\nullthrows(Keywords::$all->get($value));
  }
}

}
