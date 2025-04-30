#pragma once

#include <vector>
#include <unordered_map>
#include "Token.hpp"

class Scanner {
  private:
    std::string::size_type start = 0;
    std::string::size_type current = 0;
    std::string::size_type line = 1;
    std::string source;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords = {
      {"include",TokenType::INCLUDE},
      {"and",    TokenType::AND},
      {"class",  TokenType::CLASS},
      {"else",   TokenType::ELSE},
      {"false",  TokenType::FALSE},
      {"for",    TokenType::FOR},
      {"set",    TokenType::SET},
      {"if",     TokenType::IF},
      {"nil",    TokenType::NIL},
      {"or",     TokenType::OR},
      {"out",     TokenType::OUT},
      {"output",  TokenType::OUTPUT},
      {"return", TokenType::RETURN},
      {"super",  TokenType::SUPER},
      {"this",   TokenType::THIS},
      {"true",   TokenType::TRUE},
      {"auto",    TokenType::AUTO},
      {"while",  TokenType::WHILE}
    };

    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAlphaNumeric(char c);
    bool match(char expected);
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    char peek();
    char peekNext();
    void string();
    void number();
    void identifier();

  public:
    Scanner(const std::string&);
    bool isAtEnd();
    std::vector<Token> scanTokens();
};
