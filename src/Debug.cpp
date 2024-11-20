#include "Debug.hpp"

void Debug::report(int line, const std::string& where, const std::string& message){
  hadError = true;
  //std::cerr << "[" + Debug::filename + "] " << "error: line: " << line << where << ": " << message << '\n';
  std::cerr << "error: line: " << line << where << ": " << message << '\n';
} 

void Debug::error(int line, const std::string& message){
  report(line, "", message);
}

void Debug::error(Token token, const std::string& message){
  if(token.type == TokenType::TER_EOF){
    report(token.line, " at end ", message);
  }else{
    report(token.line, " at " + token.lexeme, message);
  }
}

void Debug::runtimeError(const RuntimeError& error){
  //std::cerr << "[" + Debug::filename + "] " << "[line " << error.token.line << "] Error: " << error.what() << '\n';
  std::cerr << "[line " << error.token.line << "] Error: " << error.what() << '\n';
}
