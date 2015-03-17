<?hh // strict

require_once 'TokenType.hh';

class Keywords
{
  const string ABSTRACT = 'abstract';
  const string ARRAYKEY = 'arraykey';
  const string AS = 'as';
  const string ASYNC = 'async';
  const string BREAK = 'break';
  const string CASE = 'case';
  const string CATCH = 'catch';
  const string CLASS = 'class';
  const string CLONE = 'clone';
  const string CONST = 'const';
  const string CONTINUE = 'continue';
  const string DEFAULT = 'default';
  const string DO = 'do';
  const string ECHO = 'echo';
  const string ELSE = 'else';
  const string ELSEIF = 'elseif';
  const string ENUM = 'enum';
  const string EXTENDS = 'extends';
  const string FINAL = 'final';
  const string FINALLY = 'finally';
  const string FOR = 'for';
  const string FOREACH = 'foreach';
  const string FUNCTION = 'function';
  const string IF = 'if';
  const string IMPLEMENTS = 'implements';
  const string INSTANCEOF = 'instanceof';
  const string INSTEADOF = 'insteadof';
  const string INTERFACE = 'interface';
  const string MIXED = 'mixed';
  const string NAMESPACE = 'namespace';
  const string NEW = 'new';
  const string NEWTYPE = 'newtype';
  const string NUM = 'num';
  const string PRIVATE = 'private';
  const string PROTECTED = 'protected';
  const string PUBLIC = 'public';
  const string REQUIRE = 'require';
  const string REQUIRE_ONCE = 'require_once';
  const string RETURN = 'return';
  const string SHAPE = 'shape';
  const string STATIC = 'static';
  const string SWITCH = 'switch';
  const string THROW = 'throw';
  const string TRAIT = 'trait';
  const string TRY = 'try';
  const string TUPLE = 'tuple';
  const string TYPE = 'type';
  const string USE = 'use';
  const string WHILE = 'while';
  const string YIELD = 'yield';

  const string FALSE = 'false';
  const string NULL = 'null';
  const string TRUE = 'true';

  private static Map<string, TokenKind> $all = Map {};

  private static function initialize(): void
  {
    Keywords::$all->set(Keywords::ABSTRACT, TokenKind::ABSTRACT);
    Keywords::$all->set(Keywords::ARRAYKEY, TokenKind::ARRAYKEY);
    Keywords::$all->set(Keywords::AS, TokenKind::AS);
    Keywords::$all->set(Keywords::ASYNC, TokenKind::ASYNC);
    Keywords::$all->set(Keywords::BREAK, TokenKind::BREAK);
    Keywords::$all->set(Keywords::CASE, TokenKind::CASE);
    Keywords::$all->set(Keywords::CATCH, TokenKind::CATCH);
    Keywords::$all->set(Keywords::CLASS, TokenKind::CLASS);
    Keywords::$all->set(Keywords::CLONE, TokenKind::CLONE);
    Keywords::$all->set(Keywords::CONST, TokenKind::CONST);
    Keywords::$all->set(Keywords::CONTINUE, TokenKind::CONTINUE);
    Keywords::$all->set(Keywords::DEFAULT, TokenKind::DEFAULT);
    Keywords::$all->set(Keywords::DO, TokenKind::DO);
    Keywords::$all->set(Keywords::ECHO, TokenKind::ECHO);
    Keywords::$all->set(Keywords::ELSE, TokenKind::ELSE);
    Keywords::$all->set(Keywords::ELSEIF, TokenKind::ELSEIF);
    Keywords::$all->set(Keywords::ENUM, TokenKind::ENUM);
    Keywords::$all->set(Keywords::EXTENDS, TokenKind::EXTENDS);
    Keywords::$all->set(Keywords::FINAL, TokenKind::FINAL);
    Keywords::$all->set(Keywords::FINALLY, TokenKind::FINALLY);
    Keywords::$all->set(Keywords::FOR, TokenKind::FOR);
    Keywords::$all->set(Keywords::FOREACH, TokenKind::FOREACH);
    Keywords::$all->set(Keywords::FUNCTION, TokenKind::FUNCTION);
    Keywords::$all->set(Keywords::IF, TokenKind::IF);
    Keywords::$all->set(Keywords::IMPLEMENTS, TokenKind::IMPLEMENTS);
    Keywords::$all->set(Keywords::INSTANCEOF, TokenKind::INSTANCEOF);
    Keywords::$all->set(Keywords::INSTEADOF, TokenKind::INSTEADOF);
    Keywords::$all->set(Keywords::INTERFACE, TokenKind::INTERFACE);
    Keywords::$all->set(Keywords::MIXED, TokenKind::MIXED);
    Keywords::$all->set(Keywords::NAMESPACE, TokenKind::NAMESPACE);
    Keywords::$all->set(Keywords::NEW, TokenKind::NEW);
    Keywords::$all->set(Keywords::NEWTYPE, TokenKind::NEWTYPE);
    Keywords::$all->set(Keywords::NUM, TokenKind::NUM);
    Keywords::$all->set(Keywords::PRIVATE, TokenKind::PRIVATE);
    Keywords::$all->set(Keywords::PROTECTED, TokenKind::PROTECTED);
    Keywords::$all->set(Keywords::PUBLIC, TokenKind::PUBLIC);
    Keywords::$all->set(Keywords::REQUIRE, TokenKind::REQUIRE);
    Keywords::$all->set(Keywords::REQUIRE_ONCE, TokenKind::REQUIRE_ONCE);
    Keywords::$all->set(Keywords::RETURN, TokenKind::RETURN);
    Keywords::$all->set(Keywords::SHAPE, TokenKind::SHAPE);
    Keywords::$all->set(Keywords::STATIC, TokenKind::STATIC);
    Keywords::$all->set(Keywords::SWITCH, TokenKind::SWITCH);
    Keywords::$all->set(Keywords::THROW, TokenKind::THROW);
    Keywords::$all->set(Keywords::TRAIT, TokenKind::TRAIT);
    Keywords::$all->set(Keywords::TRY, TokenKind::TRY);
    Keywords::$all->set(Keywords::TUPLE, TokenKind::TUPLE);
    Keywords::$all->set(Keywords::TYPE, TokenKind::TYPE);
    Keywords::$all->set(Keywords::USE, TokenKind::USE);
    Keywords::$all->set(Keywords::WHILE, TokenKind::WHILE);
    Keywords::$all->set(Keywords::YIELD, TokenKind::YIELD);

    Keywords::$all->set(Keywords::FALSE, TokenKind::FALSE);
    Keywords::$all->set(Keywords::NULL, TokenKind::NULL);
    Keywords::$all->set(Keywords::TRUE, TokenKind::TRUE);
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
