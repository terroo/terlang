#include "Scanner.hpp"

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
  return current >= static_cast<int>(source.length());
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

  std::string text = source.substr(start, current - start);
  double number = std::stod(std::string{text});
  addToken(TokenType::NUMBER, number);
}

void Scanner::string(){
  while(peek() != '"' && !isAtEnd()){
    if(peek() == '\n'){ line++; }
    advance();
  }

  if(isAtEnd()){
    Debug::error(line, "Unterminated string.");
    return;
  }

  advance();

  std::string value(source.substr(start + 1, current - start - 2));
  addToken(TokenType::STRING, value);
}

bool Scanner::match(char expected){
  if(isAtEnd() || source.at(current) != expected) return false;
  current++;
  return true;
}

char Scanner::peek(){
  if(isAtEnd()) return '\0';
  return source.at(current);
}

char Scanner::peekNext(){
  if(current + 1 > static_cast<int>(source.length())) return '\0';
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

char Scanner::advance(){
  return source[current++];
}

void Scanner::scanToken(){
  char c = advance();
  switch(c){
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
              addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
    case '>':
              addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
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
