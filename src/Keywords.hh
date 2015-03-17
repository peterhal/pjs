<?hh // strict

require_once 'TokenKind.hh';

class Keywords
{
  const string KW_ABSTRACT = 'abstract';
  const string KW_ARRAYKEY = 'arraykey';
  const string KW_AS = 'as';
  const string KW_ASYNC = 'async';
  const string KW_BREAK = 'break';
  const string KW_CASE = 'case';
  const string KW_CATCH = 'catch';
  const string KW_CLASS = 'class';
  const string KW_CLONE = 'clone';
  const string KW_CONST = 'const';
  const string KW_CONTINUE = 'continue';
  const string KW_DEFAULT = 'default';
  const string KW_DO = 'do';
  const string KW_ECHO = 'echo';
  const string KW_ELSE = 'else';
  const string KW_ELSEIF = 'elseif';
  const string KW_ENUM = 'enum';
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
  const string KW_MIXED = 'mixed';
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

  const string KW_FALSE = 'false';
  const string KW_NULL = 'null';
  const string KW_TRUE = 'true';

  private static Map<string, TokenKind> $all = Map {};

  private static function initialize(): void
  {
    Keywords::$all->set(Keywords::KW_ABSTRACT, TokenKind::KW_ABSTRACT);
    Keywords::$all->set(Keywords::KW_ARRAYKEY, TokenKind::KW_ARRAYKEY);
    Keywords::$all->set(Keywords::KW_AS, TokenKind::KW_AS);
    Keywords::$all->set(Keywords::KW_ASYNC, TokenKind::KW_ASYNC);
    Keywords::$all->set(Keywords::KW_BREAK, TokenKind::KW_BREAK);
    Keywords::$all->set(Keywords::KW_CASE, TokenKind::KW_CASE);
    Keywords::$all->set(Keywords::KW_CATCH, TokenKind::KW_CATCH);
    Keywords::$all->set(Keywords::KW_CLASS, TokenKind::KW_CLASS);
    Keywords::$all->set(Keywords::KW_CLONE, TokenKind::KW_CLONE);
    Keywords::$all->set(Keywords::KW_CONST, TokenKind::KW_CONST);
    Keywords::$all->set(Keywords::KW_CONTINUE, TokenKind::KW_CONTINUE);
    Keywords::$all->set(Keywords::KW_DEFAULT, TokenKind::KW_DEFAULT);
    Keywords::$all->set(Keywords::KW_DO, TokenKind::KW_DO);
    Keywords::$all->set(Keywords::KW_ECHO, TokenKind::KW_ECHO);
    Keywords::$all->set(Keywords::KW_ELSE, TokenKind::KW_ELSE);
    Keywords::$all->set(Keywords::KW_ELSEIF, TokenKind::KW_ELSEIF);
    Keywords::$all->set(Keywords::KW_ENUM, TokenKind::KW_ENUM);
    Keywords::$all->set(Keywords::KW_EXTENDS, TokenKind::KW_EXTENDS);
    Keywords::$all->set(Keywords::KW_FINAL, TokenKind::KW_FINAL);
    Keywords::$all->set(Keywords::KW_FINALLY, TokenKind::KW_FINALLY);
    Keywords::$all->set(Keywords::KW_FOR, TokenKind::KW_FOR);
    Keywords::$all->set(Keywords::KW_FOREACH, TokenKind::KW_FOREACH);
    Keywords::$all->set(Keywords::KW_FUNCTION, TokenKind::KW_FUNCTION);
    Keywords::$all->set(Keywords::KW_IF, TokenKind::KW_IF);
    Keywords::$all->set(Keywords::KW_IMPLEMENTS, TokenKind::KW_IMPLEMENTS);
    Keywords::$all->set(Keywords::KW_INSTANCEOF, TokenKind::KW_INSTANCEOF);
    Keywords::$all->set(Keywords::KW_INSTEADOF, TokenKind::KW_INSTEADOF);
    Keywords::$all->set(Keywords::KW_INTERFACE, TokenKind::KW_INTERFACE);
    Keywords::$all->set(Keywords::KW_MIXED, TokenKind::KW_MIXED);
    Keywords::$all->set(Keywords::KW_NAMESPACE, TokenKind::KW_NAMESPACE);
    Keywords::$all->set(Keywords::KW_NEW, TokenKind::KW_NEW);
    Keywords::$all->set(Keywords::KW_NEWTYPE, TokenKind::KW_NEWTYPE);
    Keywords::$all->set(Keywords::KW_NUM, TokenKind::KW_NUM);
    Keywords::$all->set(Keywords::KW_PRIVATE, TokenKind::KW_PRIVATE);
    Keywords::$all->set(Keywords::KW_PROTECTED, TokenKind::KW_PROTECTED);
    Keywords::$all->set(Keywords::KW_PUBLIC, TokenKind::KW_PUBLIC);
    Keywords::$all->set(Keywords::KW_REQUIRE, TokenKind::KW_REQUIRE);
    Keywords::$all->set(Keywords::KW_REQUIRE_ONCE, TokenKind::KW_REQUIRE_ONCE);
    Keywords::$all->set(Keywords::KW_RETURN, TokenKind::KW_RETURN);
    Keywords::$all->set(Keywords::KW_SHAPE, TokenKind::KW_SHAPE);
    Keywords::$all->set(Keywords::KW_STATIC, TokenKind::KW_STATIC);
    Keywords::$all->set(Keywords::KW_SWITCH, TokenKind::KW_SWITCH);
    Keywords::$all->set(Keywords::KW_THROW, TokenKind::KW_THROW);
    Keywords::$all->set(Keywords::KW_TRAIT, TokenKind::KW_TRAIT);
    Keywords::$all->set(Keywords::KW_TRY, TokenKind::KW_TRY);
    Keywords::$all->set(Keywords::KW_TUPLE, TokenKind::KW_TUPLE);
    Keywords::$all->set(Keywords::KW_TYPE, TokenKind::KW_TYPE);
    Keywords::$all->set(Keywords::KW_USE, TokenKind::KW_USE);
    Keywords::$all->set(Keywords::KW_WHILE, TokenKind::KW_WHILE);
    Keywords::$all->set(Keywords::KW_YIELD, TokenKind::KW_YIELD);

    Keywords::$all->set(Keywords::KW_FALSE, TokenKind::KW_FALSE);
    Keywords::$all->set(Keywords::KW_NULL, TokenKind::KW_NULL);
    Keywords::$all->set(Keywords::KW_TRUE, TokenKind::KW_TRUE);
  }

  public static function isKeyword(string $value): bool
  {
    if (Keywords::$all->isEmpty()) {
      Keywords::initialize();
    }
    return Keywords::$all->containsKey($value);
  }

  public static function getKeyword(string $value): TokenKind
  {
    // TODO
    return TokenKind::EOF;
    // return Keywords::$all->get($value);
  }
}
