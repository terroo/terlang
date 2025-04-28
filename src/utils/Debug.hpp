#pragma once
#include <string>
#include "../tokenizer/Token.hpp"
#include "RuntimeError.hpp"

class Debug {
  private:
    static void report(int, const std::string&, const std::string&);

  public:
    inline static std::string filename;
    inline static bool hadError = false;
    inline static bool hadRuntimeError = false;
    static void error(int line, const std::string&);
    static void error(Token token, const std::string&);
    static void runtimeError(const RuntimeError& error);
};
