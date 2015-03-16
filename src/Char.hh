<?hh // strict

class Char
{
  const int EOF = -1;

  const int HORIZONTAL_TAB =0x09;
  const int LINE_FEED = 0x0A;
  const int CARRIAGE_RETURN = 0x0D;
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
  const int COLON = 0x3A; // :
  const int SEMI_COLON = 0x3B; // ;
  const int OPEN_ANGLE = 0x3C; // <
  const int EQUAL = 0x3D; // =
  const int CLOSE_ANGLE = 0x3E; // >
  const int QUESTION = 0x3F; // ?
  const int AT = 0x40; // @
  const int OPEN_SQUARE = 0x5B; // [
  const int BACK_SLASH = 0x5C; // \
  const int CLOSE_SQUARE = 0x5D; // ]
  const int HAT = 0x5E; // ^
  const int UNDERSCORE = 0x5F; // _
  const int OPEN_CURLY = 0x7B; // {
  const int BAR = 0x7C; // |
  const int CLOSE_CURLY = 0x7D; // }
  const int TILDE = 0x7E; // ~

  public static function isNewLine(int $ch): bool
  {
    return $ch == Char::CARRIAGE_RETURN
      || $ch == Char::LINE_FEED;
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
}
