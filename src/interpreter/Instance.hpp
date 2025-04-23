#pragma once

#include "Callable.hpp"
#include "Interpreter.hpp"
#include <memory>
#include <unordered_map>

class Class;

class Instance : public Callable {
  public:
    Instance(std::shared_ptr<Class> klass);

    std::shared_ptr<Class> klass;
    std::unordered_map<std::string, std::any> fields;

    std::any get(const Token& name);
    void set(const Token& name, std::any value);

    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};
