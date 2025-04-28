#include <iostream>

#include "Environment.hpp"
#include "../utils/RuntimeError.hpp"

Env::Env() : enclosing{nullptr} {}

Env::Env(std::shared_ptr<Env> enclosing ) : enclosing{std::move(enclosing)} {}

void Env::define(const std::string& name, std::any value){
  auto elem = values.find(name);
  if(elem != values.end()){
    std::cerr << "[Error]: the name '" + name + "' for identifier was repeated.\n";
    std::exit(65);
  }

  values[name] = std::move(value);
}

std::any Env::get(const Token& name){
  auto elem = values.find(name.lexeme);
  if(elem != values.end()){
    return elem->second;
  }

  if(enclosing != nullptr){
    return enclosing->get(name);
  }

  throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'.");
}


void Env::assign(const Token& name, std::any value){
  auto elem = values.find(name.lexeme);
  if(elem != values.end()){
    elem->second = std::move(value);
    return;
  }

  if(enclosing != nullptr){
    return enclosing->assign(name, std::move(value));
  }

  throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'.");
}

std::any Env::getAt(int distance, const std::string& name){
  return anchestor(distance)->values[name];
}

void Env::assignAt(int distance, Token& name, std::any value){
  anchestor(distance)->values[name.lexeme] = std::move(value);
}

std::shared_ptr<Env> Env::anchestor(int distance){
  std::shared_ptr<Env> currentEnv = shared_from_this();
  for(int i = 0; i < distance; i++){
    currentEnv = currentEnv->enclosing;
  }
  return currentEnv;
}
