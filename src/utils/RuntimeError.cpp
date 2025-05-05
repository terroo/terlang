#include "RuntimeError.hpp"

RuntimeError::RuntimeError(const Token& token, const std::string& message) :
   std::runtime_error{message}, token{token} {}
