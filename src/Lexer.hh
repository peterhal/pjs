<?hh //strict

require_once 'Char.hh';
require_once 'ErrorReporter.hh';
require_once 'LexerBase.hh';
require_once 'Location.hh';
require_once 'SourceFile.hh';

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
    return Vector{};
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

    case Char::OPEN_SQUARE: return $create(TokenKind::OPEN_SQUARE);
    case Char::CLOSE_SQUARE: return $create(TokenKind::CLOSE_SQUARE);
    case Char::OPEN_PAREN: return $create(TokenKind::OPEN_PAREN);
    case Char::CLOSE_PAREN: return $create(TokenKind::CLOSE_PAREN);
    case Char::OPEN_CURLY: return $create(TokenKind::OPEN_CURLY);
    case Char::CLOSE_CURLY: return $create(TokenKind::CLOSE_CURLY);
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
    case Char::DOLLAR: return $create(TokenKind::DOLLAR);
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
        if ($peekChar(Char::EQUAL)) {
          $next();
          return $create(TokenKind::LEFT_SHIFT_EQUAL);
        }
        return $create(TokenKind::LEFT_SHIFT);
      case Char::EQUAL: return $create(TokenKind::LESS_EQUAL);
      default: return $create(TokenKind::OPEN_ANGLE);
      }
    case Char::CLOSE_ANGLE:
      switch ($peek())
      {
      case Char::EQUAL: $next(); return $create(TokenKind::GREATER_EQUAL);
      case Char::CLOSE_ANGLE:
        $next();
        if ($peekChar(Char::EQUAL)) {
          $next();
          return $create(TokenKind::RIGHT_SHIFT_EQUAL);
        }
        return $create(TokenKind::RIGHT_SHIFT);
      default: return $create(TokenKind::CLOSE_ANGLE);
      }
    case Char::EQUAL:
      if ($peekChar(Char::EQUAL)) {
        $next();
        if ($peekChar(Char::EQUAL)) {
          $next();
          return $create(TokenKind::EQUAL_EQUAL_EQUAL);
        }
        return $create(TokenKind::EQUAL_EQUAL);
      }
      return $create(TokenKind::EQUAL);
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
      case Char::QUESTION: return $create(TokenKind::QUESTION);
      case Char::COLON: return $create(TokenKind::COLON);
      case Char::SEMI_COLON: return $create(TokenKind::SEMI_COLON);
      case Char::COMMA: return $create(TokenKind::COMMA);
    }

    return new Token($this->location(), TokenKind::EOF);
  }

  private function createToken(int $start, TokenKind $kind): Token
  {
    return new Token($this->createRange($start), $kind);
  }

  private function createRange(int $start): Location
  {
    // TODO: Range
    return $this->file->location($start); 
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
}

