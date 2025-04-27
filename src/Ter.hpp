#pragma once

#include <string>

class Ter {
  private: 
    static void run(const std::string&);

  public:
    static void run_file(const std::string&);
    static void run_script(const std::string&);
    static void repl();
};
