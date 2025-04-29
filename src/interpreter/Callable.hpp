#pragma once

#include <vector>
#include <any>
#include <string>

class Interpreter;

class Callable {
  public:
    virtual int arity() = 0;
    virtual std::any call(Interpreter &interpreter,
            std::vector<std::any> arguments) = 0;
    virtual std::string toString() = 0;
    virtual ~Callable() = default;
};

