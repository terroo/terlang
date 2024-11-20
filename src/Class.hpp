#pragma once

#include "Interpreter.hpp"
#include "Stmt.hpp"
#include "Callable.hpp"
#include "Environment.hpp"
#include <optional>
#include "Function.hpp"

class Instance;

class Class : public Callable{
  public:
    Class(const std::string& name, std::unordered_map<std::string, std::shared_ptr<Function>> methods);
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Function>> methods;
    std::optional<std::shared_ptr<Function>> findMethod(const std::string& name);

    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};
