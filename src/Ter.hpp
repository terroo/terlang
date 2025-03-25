#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Resolver.hpp"

class Ter {
  private: 
    static void run(const std::string&);

  public:
    static void run_file(const std::string&);
    static void run_script(const std::string&);
    static void repl();
};
