#include "Token.hpp"
#include <sstream>

Token::Token(TokenType type, std::string lexeme, std::any literal, int line) : 
  type(type), lexeme(lexeme), literal(literal), line(line) {}

std::string Token::toString(){
  std::stringstream ss_literal;
  if(literal.has_value()){
    const std::type_info& type_any = literal.type();

    if(type_any == typeid(std::string)){
      ss_literal << std::any_cast<std::string>(literal);
    }else if(type_any == typeid(int)){
      ss_literal << std::any_cast<int>(literal);
    }else if(type_any == typeid(double)){
      ss_literal << std::any_cast<double>(literal);
    }else{
      ss_literal << "null";
    }
  }else{
    ss_literal << "[no literal]";
  }
  return lexeme + " " + ss_literal.str();
}

bool Token::operator <(const Token& obj) const{
  return obj.lexeme < this->lexeme;
}
