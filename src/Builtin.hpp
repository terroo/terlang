#pragma once
#include "Callable.hpp"
#include "Debug.hpp"
#include "Interpreter.hpp"

class Clock : public Callable {
  public:
    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};

class Rand : public Callable {
  public:
    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};

class GetEnv : public Callable {
  public:
    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};

class ToString : public Callable {
  public:
    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};

class Args : public Callable {
  public:
    int arity() override;
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
    std::string toString() override;
};
