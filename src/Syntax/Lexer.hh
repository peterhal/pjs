<?hh //strict

namespace Syntax {

require_once 'Utils/Char.hh';
require_once 'Utils/ErrorReporter.hh';
require_once 'Keywords.hh';
require_once 'LexerBase.hh';
require_once 'Utils/Location.hh';
require_once 'Utils/SourceFile.hh';
require_once 'Token.hh';

use Utils\Char;
use Utils\ErrorReporter;
use Utils\SourceFile;
use Utils\Location;

class Lexer extends LexerBase
{
  public static function lexFile(
    SourceFile $file,
    ErrorReporter $reporter): Vector<Token>
  {
    return (new Lexer($file, $reporter))->lexAll();
  }

  public function __construct(
    SourceFile $file,
    ErrorReporter $reporter)
  {
    parent::__construct($file, $reporter);
  }

  public function lexAll(): Vector<Token>
  {
    $this->skipWhitespaceAndComments();
    $result = Vector{};
    while (!$this->eof()) {
      $result->add($this->nextToken());
    }
    return $result;
  }

  public function nextToken(): Token
  {
    $this->skipWhitespaceAndComments();
    $start = $this->offset;
    $peek = () ==> $this->peek();
    $peekChar = $char ==> $this->peekChar($char);
    $next = () ==> $this->next();
    $ch = $next();

    $create = $kind ==> $this->createToken($start, $kind);
    switch ($ch)
    {
    case Char::EOF: return $create(TokenKind::EOF);

    case Char::AT: return $create(TokenKind::AT); // TODO: Spec - missing.
    case Char::OPEN_SQUARE: return $create(TokenKind::OPEN_SQUARE);
    case Char::CLOSE_SQUARE: return $create(TokenKind::CLOSE_SQUARE);
    case Char::OPEN_PAREN: return $create(TokenKind::OPEN_PAREN);
    case Char::CLOSE_PAREN: return $create(TokenKind::CLOSE_PAREN);
    case Char::OPEN_CURLY: return $create(TokenKind::OPEN_CURLY);
    case Char::CLOSE_CURLY: return $create(TokenKind::CLOSE_CURLY);
    case Char::BACK_SLASH: return $create(TokenKind::BACK_SLASH);
    case Char::PERIOD: 
      if ($peekChar(Char::EQUAL)) {
        $next();
        return $create(TokenKind::PERIOD_EQUAL);
      }
      return $create(TokenKind::PERIOD);
    case Char::MINUS:
      switch ($peek())
      {
      case Char::CLOSE_ANGLE: $next(); return $create(TokenKind::ARROW);
      case Char::MINUS: $next(); return $create(TokenKind::MINUS_MINUS);
      case Char::EQUAL: $next(); return $create(TokenKind::MINUS_EQUAL);
      default: return $create(TokenKind::MINUS);
      }
    case Char::PLUS:
      switch ($peek())
      {
      case Char::PLUS: $next(); return $create(TokenKind::PLUS_PLUS);
      case Char::EQUAL: $next(); return $create(TokenKind::PLUS_EQUAL);
      default: return $create(TokenKind::PLUS);
      }
    case Char::STAR:
      switch ($peek())
      {
      case Char::STAR:
        $next();
        if ($peekChar(Char::EQUAL)) {
          $next();
          return $create(TokenKind::STAR_STAR_EQUAL);
        }
        return $create(TokenKind::PLUS_PLUS);
      case Char::EQUAL: $next(); return $create(TokenKind::STAR_EQUAL);
      default: return $create(TokenKind::STAR);
      }
    case Char::TILDE: return $create(TokenKind::TILDE);
    case Char::BANG:
      if ($peekChar(Char::EQUAL)) {
        $next();
        if ($peekChar(Char::EQUAL)) {
          $next();
          return $create(TokenKind::BANG_EQUAL_EQUAL);
        }
        return $create(TokenKind::BANG_EQUAL);
      }
      return $create(TokenKind::BANG);
    case Char::DOLLAR: 
      if (Char::isNameNonDigit($peek())) {
        return $this->lexVariableName($start);
      }
      return $create(TokenKind::DOLLAR);
    case Char::FORWARD_SLASH:
      if ($peekChar(Char::EQUAL)) {
        $next();
        return $create(TokenKind::SLASH_EQUAL);
      }
      return $create(TokenKind::FORWARD_SLASH);
    case Char::PERCENT:
      if ($peekChar(Char::EQUAL)) {
        $next();
        return $create(TokenKind::PERCENT_EQUAL);
      }
      return $create(TokenKind::PERCENT);
    case Char::OPEN_ANGLE:
      switch ($peek()) 
      {
      case Char::OPEN_ANGLE:
        $next();
        switch ($peek()) {
        case Char::OPEN_ANGLE:
          $next();
          return $this->lexMultiLineString($start);
        case Char::EQUAL:
          $next();
          return $create(TokenKind::LEFT_SHIFT_EQUAL);
        }
        return $create(TokenKind::LEFT_SHIFT);
        case Char::EQUAL: 
          $next() ;
          return $create(TokenKind::LESS_EQUAL);
        default: 
          return $create(TokenKind::OPEN_ANGLE);
      }
    case Char::CLOSE_ANGLE:
      // >> and >>= are handled specially in the parser
      // to avoid ambiguity with generic types
      if ($peekChar(Char::EQUAL)) {
        $next(); 
        return $create(TokenKind::GREATER_EQUAL);
      }
      return $create(TokenKind::CLOSE_ANGLE);
    case Char::EQUAL:
      switch ($peek()) {
      case Char::CLOSE_ANGLE:
        $next();
        return $create(TokenKind::FAT_ARROW);
      case Char::EQUAL:
        $next();
        switch ($peek()) {
        case Char::CLOSE_ANGLE:
          $next();
          return $create(TokenKind::FATTER_ARROW);
        case Char::EQUAL:
          $next();
          return $create(TokenKind::EQUAL_EQUAL_EQUAL);
        default:
          return $create(TokenKind::EQUAL_EQUAL);
        }
      default:
        return $create(TokenKind::EQUAL);
      }
    case Char::HAT:
      if ($peekChar(Char::EQUAL)) {
        $next();
        return $create(TokenKind::HAT_EQUAL);
      }
      return $create(TokenKind::HAT);
    case Char::BAR:
      switch ($peek())
      {
      case Char::BAR: $next(); return $create(TokenKind::BAR_BAR);
      case Char::EQUAL: $next(); return $create(TokenKind::BAR_EQUAL);
      default: return $create(TokenKind::BAR);
      }
    case Char::AMPERSAND:
      switch ($peek())
      {
      case Char::AMPERSAND: $next(); return $create(TokenKind::AMPERSAND_AMPERSAND);
      case Char::EQUAL: $next(); return $create(TokenKind::AMPERSAND_EQUAL);
      default: return $create(TokenKind::AMPERSAND);
      }
    case Char::QUESTION: 
      if ($peekChar(Char::MINUS) 
          && $this->peekCharOffset(Char::CLOSE_ANGLE, 1)) {
        $next();
        $next();
        return $create(TokenKind::NULL_SAFE_ARROW);
      }
      return $create(TokenKind::QUESTION);
    case Char::COLON: 
      if ($peekChar(Char::COLON)) {
        $next();
        return $create(TokenKind::COLON_COLON);
      }
      return $create(TokenKind::COLON);
    case Char::SEMI_COLON: return $create(TokenKind::SEMI_COLON);
    case Char::COMMA: return $create(TokenKind::COMMA);
    case Char::SINGLE_QUOTE:
      return $this->lexSingleQuotedString($start);
    case Char::DOUBLE_QUOTE:
      return $this->lexDoubleQuotedString($start);
    default:
      if (Char::isNameNonDigit($ch)) {
        return $this->lexName($start, $ch);
      } elseif (Char::isDigit($ch)) {
        return $this->lexNumber($start, $ch);
      } else {
        $this->errorOffset($start, 'Unexpected char \''.$ch.'\'.');
        return new Token($this->currentRange($start), TokenKind::EOF);
      }
    }
  }

  private function lexMultiLineString(int $start): Token
  {
    $this->skipWhitespace();
    if ($this->locationOfOffset($start)->line() !== $this->location()->line()) {
      $this->error("Here doc start identifier expected before new line.");
      return $this->createToken($start, TokenKind::MULTI_LINE_STRING);
    }

    $startIdentifier = $this->getLine();
    if (strlen($startIdentifier) === 0) {
      $this->error("Here doc start identifier expected before new line.");
      return $this->createToken($start, TokenKind::MULTI_LINE_STRING);
    }
    if (strlen($startIdentifier) > 2 
        && $startIdentifier[0] === "'"
        && $startIdentifier[strlen($startIdentifier) - 1] === "'") {
      $startIdentifier = substr(
        $startIdentifier, 1, strlen($startIdentifier) - 2);
      $isNowDoc = true;
    } else {
      $isNowDoc = false;
    }

    $value = '';
    // TODO: Validate that startIdentifier is a valid name
    do {
      $this->skipNewLine();
      $line = $this->getLine();
      $foundEnd = $line === $startIdentifier
        || $line === ($startIdentifier . ';');
      if (!$foundEnd) {
        $value .= $isNowDoc ? $line . "\n" : $this->escapeLine($line);
      }
    } while (!$foundEnd && !$this->eof());
    if (!$foundEnd) {
      $this->errorOffset($start, "Missing multi line comment terminator.");
    }
    if ($this->eof()) {
      $this->error("Expected new line after multi line comment.");
    } else {
      $this->skipNewLine();
    }

    return new StringLiteralToken(
      $this->currentRange($start),
      TokenKind::MULTI_LINE_STRING,
      $value);
  }

  private function escapeLine(string $line): string
  {
    $result = '';
    for ($index = 0; $index < count($line); $index++) {
      $ch = ord($line[$index]);
      if ($ch === Char::BACK_SLASH) {
        switch ($this->peek()) {
        case Char::BACK_SLASH:
          $index++;
          $ch = Char::BACK_SLASH;
          break;
        case Char::DOLLAR:
          $index++;
          $ch = Char::DOLLAR;
          break;
        case Char::e:
          $index++;
          $ch = Char::ESCAPE;
          break;
        case Char::f:
          $index++;
          $ch = Char::FORM_FEED;
          break;
        case Char::n:
          $index++;
          $ch = Char::LINE_FEED;
          break;
        case Char::r:
          $index++;
          $ch = Char::CARRIAGE_RETURN;
          break;
        case Char::t:
          $index++;
          $ch = Char::HORIZONTAL_TAB;
          break;
        case Char::v:
          $index++;
          $ch = Char::VERTICAL_TAB;
          break;
        case Char::x:
        case Char::X:
          throw new \Exception('Hex escapes');
        case Char::ZERO:
        case Char::ONE:
        case Char::TWO:
        case Char::THREE:
        case Char::FOUR:
        case Char::FIVE:
        case Char::SIX:
        case Char::SEVEN:
          throw new \Exception('Octal escapes');
        default:
          break;
        }
      } else if ($ch === Char::DOLLAR) {
        throw new \Exception($this->location()->toString() . ' variable escapes in heredoc strings.');
      }
      $result .= chr($ch);
    }
    $result .= chr(Char::LINE_FEED);
    return $result;
  }

  private function lexDoubleQuotedString(int $start): Token
  {
    $value = '';
    while (!$this->eof() && !$this->peekChar(Char::DOUBLE_QUOTE)) {
      $ch = $this->next();
      if ($ch === Char::BACK_SLASH) {
        switch ($this->peek()) {
        case Char::DOUBLE_QUOTE:
          $this->next();
          $ch = Char::DOUBLE_QUOTE;
          break;
        case Char::BACK_SLASH:
          $this->next();
          $ch = Char::BACK_SLASH;
          break;
        case Char::DOLLAR:
          $this->next();
          $ch = Char::DOLLAR;
          break;
        case Char::e:
          $this->next();
          $ch = Char::ESCAPE;
          break;
        case Char::f:
          $this->next();
          $ch = Char::FORM_FEED;
          break;
        case Char::n:
          $this->next();
          $ch = Char::LINE_FEED;
          break;
        case Char::r:
          $this->next();
          $ch = Char::CARRIAGE_RETURN;
          break;
        case Char::t:
          $this->next();
          $ch = Char::HORIZONTAL_TAB;
          break;
        case Char::v:
          $this->next();
          $ch = Char::VERTICAL_TAB;
          break;
        case Char::x:
        case Char::X:
          throw new \Exception('Hex escapes');
        case Char::ZERO:
        case Char::ONE:
        case Char::TWO:
        case Char::THREE:
        case Char::FOUR:
        case Char::FIVE:
        case Char::SIX:
        case Char::SEVEN:
          throw new \Exception('Octal escapes');
        default:
          $this->error("Invalid double quoted string escape.");
          break;
        }
      } else if ($ch === Char::DOLLAR) {
        // TODO: variable names ...
      }
      $value .= chr($ch);
    }
    if ($this->eof()) {
      $this->errorOffset($start, "Unterminated double quoted string.");
    } else {
      $this->next();
    }

    return new StringLiteralToken(
      $this->currentRange($start), 
      TokenKind::DOUBLE_QUOTED_STRING,
      $value);
  }

  private function lexSingleQuotedString(int $start): Token
  {
    $value = '';
    while (!$this->eof() && !$this->peekChar(Char::SINGLE_QUOTE)) {
      if ($this->peekChar(Char::BACK_SLASH)) {
        $this->next();
        switch ($this->peek()) {
        case Char::BACK_SLASH:
        case Char::SINGLE_QUOTE:
          break;
        default:
          $this->error('Only \'\\\' and \'\'\' are permitted escape sequences in single quoted strings.');
          break;
        }
      }
      $value .= chr($this->next());
    }
    if ($this->eof()) {
      $this->errorOffset($start, "Unterminated single quoted string.");
    } else {
      $this->next();
    }

    return new StringLiteralToken(
      $this->currentRange($start), 
      TokenKind::SINGLE_QUOTED_STRING,
      $value);
  }

  private function lexNumber(int $start, int $firstChar): Token
  {
    if ($firstChar === Char::ZERO) {
      switch ($this->peek())
      {
      case Char::X:
      case Char::x:
        return $this->lexHexLiteral($start);
      case Char::B:
      case Char::b:
        return $this->lexBinaryLiteral($start);
      default:
        return $this->lexOctalLiteral($start);
      }
    }

    $divisor = 1;
    $value = Char::digitValue($firstChar);
    while (Char::isDigit($this->peek())) {
      $value *= 10;
      $divisor *= 10;
      $value += Char::digitValue($this->next());
    }
    if ($this->peek() === Char::PERIOD) {
      $this->next();
      while (Char::isDigit($this->peek())) {
        $value *= 10;
        $value += Char::digitValue($this->next());
      }
    }

    $exponent = 0;
    if ($this->peek() === Char::E || $this->peek() === Char::e) {
      $this->next();
      $sign = 1;
      switch ($this->peek())
      {
      case Char::PLUS:
        $this->next();
        break;
      case Char::MINUS:
        $this->next();
        $sign = -1;
        break;
      }
      if (!Char::isDigit($this->peek())) {
        $this->error("Missing exponent.");
      } else {
        $exponent = Char::digitValue($this->next());
      }
      while (Char::isDigit($this->peek())) {
        $exponent *= 10;
        $exponent += Char::digitValue($this->next());
      }
    }

    return $this->createToken($start, TokenKind::NUMBER);
  }

  private function lexHexLiteral(int $start): Token
  {
    $this->next(); // x or X
    if (!Char::isHexDigit($this->peek())) {
      $this->error("Missing hex digit.");
      $value = 0;
    } else {
      $value = Char::hexDigitValue($this->next());
    }
    while (Char::isHexDigit($this->peek())) {
      $value <<= 4;
      $value += Char::hexDigitValue($this->next());
    }
    // TODO: NumberToken
    return $this->createToken($start, TokenKind::NUMBER);
  }

  private function lexOctalLiteral(int $start): Token
  {
    $value = 0;
    while (Char::isOctalDigit($this->peek())) {
      $value <<= 3;
      $value += Char::digitValue($this->next());
    }
    // TODO: NumberToken
    return $this->createToken($start, TokenKind::NUMBER);
  }

  private function lexBinaryLiteral(int $start): Token
  {
    $this->next(); // b or B
    if (!Char::isBinaryDigit($this->peek())) {
      $this->error("Missing binary digit.");
      $value = 0;
    } else {
      $value = Char::digitValue($this->next());
    }
    while (Char::isBinaryDigit($this->peek())) {
      $value <<= 1;
      $value += Char::digitValue($this->next());
    }
    // TODO: NumberToken
    return $this->createToken($start, TokenKind::NUMBER);
  }

  private function lexVariableName(int $start): Token
  {
    $value = '$';
    while (Char::isName($this->peek())) {
      $value .= chr($this->next());
    }
    return new VariableNameToken(
      $this->currentRange($start), 
      $value);
  }

  private function lexName(int $start, int $firstChar): Token
  {
    $value = chr($firstChar);
    while (Char::isName($this->peek())) {
      $value .= chr($this->next());
    }

    if (Keywords::isKeyword($value)) {
      return $this->createToken($start, Keywords::getKeyword($value));
    }

    return new NameToken($this->currentRange($start), $value);
  }

  private function createToken(int $start, TokenKind $kind): Token
  {
    return new Token($this->currentRange($start), $kind);
  }

  private function skipWhitespaceAndComments(): void
  {
    do {
      $this->skipWhitespace();
    } while ($this->skipComment());
  }

  private function skipWhitespace(): void
  {
    while (!$this->eof() && Char::isWhitespace($this->peek())) {
      $this->next();
    }
  }

  private function skipComment(): bool
  {
    if ($this->peekChar(Char::HASH)) {
      $this->next();
      $this->skipToEndOfLine();
      return true;
    } else if ($this->peekChar(Char::FORWARD_SLASH)) {
      if ($this->peekCharOffset(Char::FORWARD_SLASH, 1)) {
        $this->next();
        $this->next();
        $this->skipToEndOfLine();
        return true;
      } else if ($this->peekCharOffset(Char::STAR, 1)) {
        $this->next();
        $this->next();
        $this->skipToEndOfDelimitedComment();
        return true;
      }
    }

    return false;
  }

  private function skipToEndOfLine(): void
  {
    while (!$this->eof() && !Char::isNewLine($this->peek())) {
      $this->next();
    }
  }
  private function skipToEndOfDelimitedComment(): void
  {
    while (true) {
      if ($this->eof()) {
        $this->error("Expected end of delimited comment.");
        return;
      }
      if ($this->peekCharOffset(Char::STAR, 0)
        && $this->peekCharOffset(Char::FORWARD_SLASH, 1)) {
        $this->next();
        $this->next();
        return;
      }
      $this->next();
    }
  }

  private function skipNewLine(): void
  {
    if (!Char::isNewLine($this->peek())) {
      $this->error("New line expected.");
    }
    if ($this->next() === Char::CARRIAGE_RETURN 
      && $this->peek() === Char::LINE_FEED) {
      $this->next();
    }
  }

  private function getLine(): string
  {
    $start = $this->offset;
    $this->skipToEndOfLine();
    $end = $this->offset;
    return $this->textOfRange($start, $end);
  }
}

}
