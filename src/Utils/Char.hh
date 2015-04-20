<?hh // strict

namespace Utils {

class Char
{
  const int EOF = -1;

  const int HORIZONTAL_TAB =0x09;
  const int LINE_FEED = 0x0A;
  const int VERTICAL_TAB = 0x0B;
  const int FORM_FEED = 0x0C;
  const int CARRIAGE_RETURN = 0x0D;
  const int ESCAPE = 0x1B;
  const int SPACE = 0x20; // ' '
  const int BANG = 0x21; // !
  const int DOUBLE_QUOTE = 0x22;
  const int HASH = 0x23; // #
  const int DOLLAR = 0x24; // $
  const int PERCENT = 0x25; // %
  const int AMPERSAND = 0x26; // &
  const int SINGLE_QUOTE = 0x27; // '
  const int OPEN_PAREN = 0x28; // (
  const int CLOSE_PAREN = 0x29; // )
  const int STAR = 0x2A; // *
  const int PLUS = 0x2B; // +
  const int COMMA = 0x2C; // ,
  const int MINUS = 0x2D; // -
  const int PERIOD = 0x2E; // .
  const int FORWARD_SLASH = 0x2F; // /
  const int ZERO = 0x30; // 0
  const int ONE = 0x31; // 1
  const int TWO = 0x32; // 1
  const int THREE = 0x33; // 1
  const int FOUR = 0x34; // 1
  const int FIVE = 0x35; // 1
  const int SIX = 0x36; // 1
  const int SEVEN = 0x37; // 7
  const int NINE = 0x39; // 9
  const int COLON = 0x3A; // :
  const int SEMI_COLON = 0x3B; // ;
  const int OPEN_ANGLE = 0x3C; // <
  const int EQUAL = 0x3D; // =
  const int CLOSE_ANGLE = 0x3E; // >
  const int QUESTION = 0x3F; // ?
  const int AT = 0x40; // @
  const int A = 0x41; // A
  const int B = 0x42; // B
  const int E = 0x45; // E
  const int F = 0x46; // F
  const int X = 0x58; // X
  const int Z = 0x5A; // Z
  const int OPEN_SQUARE = 0x5B; // [
  const int BACK_SLASH = 0x5C; // \
  const int CLOSE_SQUARE = 0x5D; // ]
  const int HAT = 0x5E; // ^
  const int UNDERSCORE = 0x5F; // _
  const int a = 0x61; // a
  const int b = 0x62; // b
  const int e = 0x65; // e
  const int f = 0x66; // f
  const int n = 0x6E; // n
  const int r = 0x72; // r
  const int t = 0x74; // t
  const int v = 0x76; // v
  const int x = 0x78; // x
  const int z = 0x7A; // z
  const int OPEN_CURLY = 0x7B; // {
  const int BAR = 0x7C; // |
  const int CLOSE_CURLY = 0x7D; // }
  const int TILDE = 0x7E; // ~

  public static function isNewLine(int $ch): bool
  {
    return $ch === Char::CARRIAGE_RETURN
      || $ch === Char::LINE_FEED;
  }

  public static function isWhitespace(int $ch): bool
  {
    switch ($ch) {
    case Char::CARRIAGE_RETURN:
    case Char::LINE_FEED:
    case Char::SPACE:
    case Char::HORIZONTAL_TAB:
      return true;
    default:
      return false;
    }
  }

  public static function isHexDigit(int $ch): bool
  {
    return Char::isDigit($ch)
      || ($ch >= Char::A && $ch <= Char::F)
      || ($ch >= Char::a && $ch <= Char::f);
  }

  public static function hexDigitValue(int $ch): int
  {
    return (Char::isDigit($ch) 
      ? Char::digitValue($ch)
      : ($ch <= Char::F 
        ? $ch - Char::F + 10
        : $ch - Char::f + 10));
  }

  public static function digitValue(int $ch) : int
  {
    return $ch - Char::ZERO;
  }

  public static function isDigit(int $ch): bool
  {
    return $ch >= Char::ZERO
      && $ch <= Char::NINE;
  }

  public static function isOctalDigit(int $ch): bool
  {
    return $ch >= Char::ZERO
      && $ch <= Char::SEVEN;
  }

  public static function isBinaryDigit(int $ch): bool
  {
    return $ch === Char::ZERO || $ch === Char::ONE;
  }

  public static function isNonDigit(int $ch): bool
  {
    return $ch === Char::UNDERSCORE
      || Char::isUpperCaseLetter($ch)
      || Char::isLowerCaseLetter($ch);
  }

  public static function isLowerCaseLetter(int $ch): bool
  {
    return $ch >= Char::a && $ch <= Char::z;
  }

  public static function isUpperCaseLetter(int $ch): bool
  {
    return $ch >= Char::A && $ch <= Char::Z;
  }

  public static function isNameNonDigit(int $ch): bool
  { 
    return Char::isNonDigit($ch)
      || ($ch >= 0x7F && $ch <= 0xFF);
  }

  public static function isName(int $ch): bool
  {
    return Char::isNameNonDigit($ch) || Char::isDigit($ch);
  }
}

}

