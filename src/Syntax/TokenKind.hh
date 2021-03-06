<?hh //strict

namespace Syntax {

enum TokenKind : int
{
  EOF = 0;

  // Operators and Punctuators
  OPEN_SQUARE = 1;
  CLOSE_SQUARE = 2;
  OPEN_PAREN = 3;
  CLOSE_PAREN = 4;
  OPEN_CURLY = 5;
  CLOSE_CURLY = 6;
  PERIOD = 7;
  ARROW = 8;
  PLUS_PLUS = 9;
  MINUS_MINUS = 10;
  STAR_STAR = 11;
  STAR = 12;
  PLUS = 13;
  MINUS = 14;
  TILDE = 15;
  BANG = 16;
  DOLLAR = 17;
  FORWARD_SLASH = 18;
  PERCENT = 19;
  LEFT_SHIFT = 20;
  RIGHT_SHIFT = 21; // Produced by the parser, not the lexer.
  OPEN_ANGLE = 22;
  CLOSE_ANGLE = 23;
  LESS_EQUAL = 24;
  GREATER_EQUAL = 25;
  EQUAL_EQUAL = 26;
  EQUAL_EQUAL_EQUAL = 27;
  BANG_EQUAL = 28;
  BANG_EQUAL_EQUAL = 29;
  HAT = 30;
  BAR = 31;
  AMPERSAND = 32;
  AMPERSAND_AMPERSAND = 33;
  BAR_BAR = 34;
  QUESTION = 35;
  COLON = 36;
  SEMI_COLON = 37;
  EQUAL = 38;
  STAR_STAR_EQUAL = 39;
  STAR_EQUAL = 40;
  SLASH_EQUAL = 41;
  PERCENT_EQUAL = 42;
  PLUS_EQUAL = 43;
  MINUS_EQUAL = 44;
  PERIOD_EQUAL = 45;
  LEFT_SHIFT_EQUAL = 46;
  RIGHT_SHIFT_EQUAL = 47; // Produced by the parser, not the lexer.
  AMPERSAND_EQUAL = 48;
  HAT_EQUAL = 49;
  BAR_EQUAL = 50;
  COMMA = 51;
  BACK_SLASH = 52;
  FAT_ARROW = 53;
  AT = 54;
  FATTER_ARROW = 55;
  COLON_COLON = 56;
  NULL_SAFE_ARROW = 57;

  // TODO: Legacy punctuation
  // ...
  // <>

  NAME = 70;
  VARIABLE_NAME = 71;
  NUMBER = 72;
  SINGLE_QUOTED_STRING = 73;
  DOUBLE_QUOTED_STRING = 74;
  MULTI_LINE_STRING = 75;

  // Keywords
  KW_ABSTRACT = 100;
  KW_ARRAYKEY = 101;
  KW_AS = 102;
  KW_ASYNC = 103;
  KW_BREAK = 104;
  KW_CASE = 105;
  KW_CATCH = 106;
  KW_CLASS = 107;
  KW_CLONE = 108;
  KW_CONST = 109;
  KW_CONTINUE = 110;
  KW_DEFAULT = 111;
  KW_DO = 112;
  KW_ECHO = 113;
  KW_ELSE = 114;
  KW_ELSEIF = 115;
  KW_ENUM = 116;
  KW_EXTENDS = 117;
  KW_FINAL = 118;
  KW_FINALLY = 119;
  KW_FOR = 120;
  KW_FOREACH = 121;
  KW_FUNCTION = 122;
  KW_IF = 123;
  KW_IMPLEMENTS = 124;
  KW_INSTANCEOF = 125;
  KW_INSTEADOF = 126;
  KW_INTERFACE = 127;
  KW_MIXED = 128;
  KW_NAMESPACE = 129;
  KW_NEW = 130;
  KW_NEWTYPE = 131;
  KW_NUM = 132;
  KW_PRIVATE = 133;
  KW_PROTECTED = 134;
  KW_PUBLIC = 135;
  KW_REQUIRE = 136;
  KW_REQUIRE_ONCE = 137;
  KW_RETURN = 138;
  KW_SHAPE = 139;
  KW_STATIC = 140;
  KW_SWITCH = 141;
  KW_THROW = 142;
  KW_TRAIT = 143;
  KW_TRY = 144;
  KW_TUPLE = 145;
  KW_TYPE = 146;
  KW_USE = 147;
  KW_WHILE = 148;
  KW_YIELD = 149;
 
  KW_FALSE = 150;
  KW_NULL = 151;
  KW_TRUE = 152;

  KW_ARRAY = 153;
  KW_AWAIT = 154;
  KW_DIE = 155;
  KW_EXIT = 156;
  KW_LIST = 157;
}

function tokenKindToString(TokenKind $kind): string
{
  return TokenKind::getNames()[$kind];
}

function isTokenKindKeyword(TokenKind $kind): bool
{
  return $kind >= TokenKind::KW_ABSTRACT
    && $kind <= TokenKind::KW_LIST;
}

}

