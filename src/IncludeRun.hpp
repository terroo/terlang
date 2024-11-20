#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Scanner.hpp"

class IncludeRun {
  public:
    static void scanFile(std::string path);
    static void run(const std::string&);
    static std::vector<Token> tokens;
    static std::vector<Token> getTokens();
};
