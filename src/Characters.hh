<?hh // strict

class Characters
{
  const int EOF = -1;

  const int HORIZONTAL_TAB =0x09;
  const int LINE_FEED = 0x0A;
  const int CARRIAGE_RETURN = 0x0D;
  const int SPACE = 0x20;

  const int HASH = 0x23; //'#'
  const int STAR = 0x2A; // '*'
  const int FORWARD_SLASH = 0x2F; // '/'

  public static function isNewLine(int $ch): bool
  {
    return $ch == Characters::CARRIAGE_RETURN
      || $ch == Characters::LINE_FEED;
  }
  public static function isWhitespace(int $ch): bool
  {
    switch ($ch) {
    case Characters::CARRIAGE_RETURN:
    case Characters::LINE_FEED:
    case Characters::SPACE:
    case Characters::HORIZONTAL_TAB:
      return true;
    default:
      return false;
    }
  }
}
