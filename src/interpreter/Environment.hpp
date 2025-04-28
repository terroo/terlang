#pragma once

#include <unordered_map>
#include <any>
#include <memory>

#include "../tokenizer/Token.hpp"

class Env : public std::enable_shared_from_this<Env> {
  private:
    std::shared_ptr<Env> enclosing;
    std::unordered_map<std::string, std::any> values;

  public:
    Env();
    Env(std::shared_ptr<Env> enclosing);
    void define(const std::string& name, std::any value);
    std::any get(const Token& name);
    void assign(const Token& name, std::any value);
    std::any getAt(int distance, const std::string& name);
    void assignAt(int distance, Token& name, std::any value);
    std::shared_ptr<Env> anchestor(int distance);
};
