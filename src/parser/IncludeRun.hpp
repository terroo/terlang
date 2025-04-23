#include <vector>

#include "Token.hpp"

class IncludeRun {
  public:
    static void scanFile(std::string path);
    static void run(const std::string&);
    static std::vector<Token> tokens;
    static std::vector<Token> getTokens();
};
