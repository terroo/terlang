#pragma once

#include <any>
#include <string>
#include "TokenType.hpp"

class Token {
  public:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, std::string, std::any, int);
    bool operator <(const Token& obj) const;
    std::string toString();
};
