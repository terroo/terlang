#include "Scanner.hpp"
#include "../utils/Debug.hpp"

Scanner::Scanner(const std::string& source) : source(source) {}

std::vector<Token> Scanner::scanTokens(){
  while(!isAtEnd()){
    start = current;
    scanToken();
  }
  tokens.emplace_back(TokenType::TER_EOF, "", nullptr, line);
  return tokens;
}

bool Scanner::isAtEnd(){
  return current >= source.length();
}

void Scanner::addToken(TokenType type){
  addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, std::any literal){
  std::string text{source.substr(start, current - start)};
  tokens.emplace_back(type, text, literal, line);
}

void Scanner::identifier(){
  while(isAlphaNumeric(peek())) advance();
  std::string text{source.substr(start, current - start)};
  auto it = keywords.find(text);
  TokenType type = it == keywords.end() ? TokenType::IDENTIFIER : it->second;
  addToken(type);
}

void Scanner::number(){
  while(isDigit(peek())) advance();

  if(peek() == '.' && isDigit(peekNext())){
    advance();
    while(isDigit(peek())) advance();
  }

  std::string text{source.substr(start, current - start)};
  double number = std::stod(std::string{text});
  addToken(TokenType::NUMBER, number);
}

/**
  Checks escaped char after \x or \u.
  They must be {HH} format.
  @returns the string escaped, or empty string if error.
 */
std::string Scanner::getUnicodeEscapedChar() {
  std::string res;
  std::string hex;
  long hexCode;
  char c1;
  char cNext;
  bool done = false;

  c1 = advance();
  if (c1 != '{') {
    // TODO Debug::warning(line,"Scanner.cpp", "Invalid escape code.");
    res += c1;
    return res;
  }
  do {
    cNext = advance();
    if (isHex(cNext)) {
      hex += cNext;
      continue;
    }
    if (cNext == '}') {
      done = true;
      break;
    }
  } while (!isAtEnd());

  if (!done) {
    Debug::error(line, "Wrong escaped character.");
    return res; 
  }

  hexCode = 0;
  if (!hex.empty()) {
    hexCode = std::stol(hex, nullptr, 16);
  }
  if (hexCode <= 0x7F) {
    res += static_cast<char>(hexCode);
  } else if (hexCode <= 0x7FF) {
    res += static_cast<char>(0xC0 | (hexCode >> 6));
    res += static_cast<char>(0x80 | (hexCode & 0x3F));
  } else if (hexCode <= 0xFFFF) {
    res += static_cast<char>(0xE0 | (hexCode >> 12));
    res += static_cast<char>(0x80 | ((hexCode >> 6) & 0x3F));
    res += static_cast<char>(0x80 | (hexCode & 0x3F));
  } else if (hexCode <= 0x10FFFF) {
    res += static_cast<char>(0xF0 | (hexCode >> 18));
    res += static_cast<char>(0x80 | ((hexCode >> 12) & 0x3F));
    res += static_cast<char>(0x80 | ((hexCode >> 6) & 0x3F));
    res += static_cast<char>(0x80 | (hexCode & 0x3F));
  }else {
    // TODO Debug::warning(line,"Scanner.cpp", "Invalid unicode escaped character.");
  }
  return  res;
}

void Scanner::string(){
  std::string res, escaped;
  bool end = false;
  char nextC, currentC;
  while(!isAtEnd() && !end){
    currentC = advance();
    switch (currentC) {
      case '"': end = true; break;
      case '\n': line++; res += '\n'; break;
      case '\\':
        nextC = advance();
        switch (nextC) {
          case '0':  res += '\0'; break;
          case 'a':  res += '\a'; break;
          case 'b':  res += '\b'; break;
          case 'f':  res += '\f'; break;
          case 'n':  res += '\n'; break;
          case 'r':  res += '\r'; break;
          case 't':  res += '\t'; break;
          case 'v':  res += '\v'; break;
          case '\\': res += '\\'; break;
          case '\'': res += '\''; break;
          case '\"': res += '\"'; break;
          case '\?': res += '?'; break;
          case 'x':
          case 'u':
              escaped = getUnicodeEscapedChar();
              if (!escaped.empty()) {
                res += escaped;
              }
              break;
          default:
              // Unknown escaped. Ignore.
              // TODO Debug::warning(line,"Scanner.cpp", "Unknow escaped code.");
              break;
        }
        break;
      default:
        res += currentC;
    }
  }

  if(isAtEnd()){
    Debug::error(line, "Unterminated string.");
    return;
  }

  addToken(TokenType::STRING, res);
}

bool Scanner::match(char expected){
  if (isAtEnd() || source.at(current) != expected) return false;
  current++;
  return true;
}

char Scanner::peek(){
  if(isAtEnd()) return '\0';
  return source.at(current);
}

char Scanner::peekNext(){
  if(current + 1 > source.length()) return '\0';
  return source.at(current + 1);
}

bool Scanner::isAlpha(char c){
  return (c >= 'a' && c <= 'z') || 
    (c >= 'A' && c <= 'Z') || 
    c == '_';
}

bool Scanner::isAlphaNumeric(char c){
  return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c){
  return c >= '0' && c <= '9';
}

bool Scanner::isHex(char c){
  return (c >= 'a' && c <= 'f') ||
    (c >= 'A' && c <= 'F') ||
    (c >= '0' && c <= '9');
}

char Scanner::advance(){
  return source[current++];
}

void Scanner::scanToken(){
  char c = advance();
  switch(c){
    case '&': addToken(TokenType::AMPERSAND); break;
    case '^': addToken(TokenType::CARET); break;
    case '|': addToken(TokenType::VBAR); break;
    case '~': addToken(TokenType::TILDE); break;
    case '%': addToken(TokenType::PERCENT); break;
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': 
              if(match('-')){
                addToken(TokenType::MINUS_MINUS);
              }else{
                addToken(TokenType::MINUS); 
              }
              break;
    case '+': 
              if(match('+')){
                addToken(TokenType::PLUS_PLUS); 
              }else{
                addToken(TokenType::PLUS); 
              }
              break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': addToken(TokenType::STAR); break;
    case '[': addToken(TokenType::LEFT_BRACKET); break;
    case ']': addToken(TokenType::RIGHT_BRACKET); break;
    case '!':
              addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
    case '=':
              addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
    case '<':
              if (match('=')) {
                addToken(TokenType::LESS_EQUAL);
              }else if (match('<')) {
                addToken(TokenType::LESS_LESS);
              }else {
                addToken(TokenType::LESS);
              }
              break;
    case '>':
              if (match('=')) {
                addToken(TokenType::GREATER_EQUAL);
              }else if (match('>')) {
                addToken(TokenType::GREATER_GREATER);
              }else {
                addToken(TokenType::GREATER);
              }
              break;
    case '/':
              if(match('/')){
                while(peek() != '\n' && !isAtEnd()){
                  advance();
                }
              }else if(match('*')){
                while(!isAtEnd()){
                  if(peek() == '\n') line++;
                  if(match('*') && peek() == '/'){
                    advance();
                    return;
                  }
                  advance();
                }
                Debug::error(line, "Expected '*/' to close multiline comment.");
              }else{
                addToken(TokenType::SLASH);
              }
              break;
    case ' ':
    case '\r':
    case '\t':
              break;
    case '\n':
              line++;
              break;
    case '"':
              string();
              break;
    default:
              if(isDigit(c)){
                number();
              }else if(isAlpha(c)){
                identifier();
              }else{
                Debug::error(line, "Unexpected character.");
              }
              break;
  } 
}
