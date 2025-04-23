#include <iostream>

#include "Instance.hpp"
#include "RuntimeError.hpp"
#include "Class.hpp"

Instance::Instance(std::shared_ptr<Class> klass) : klass{std::move(klass)} {}

std::string Instance::toString(){
  return "<" + klass->name + " class instance>";
}

std::any Instance::get(const Token& name){
  if(fields.find(name.lexeme) != fields.end()){
    return fields[name.lexeme];
  }

  auto method = klass->findMethod(name.lexeme);
  if(method.has_value()){
    return method.value();
  }

  throw RuntimeError(name, "Undefinied property '" + name.lexeme + "'.");
}

void Instance::set(const Token& name, std::any value){
  fields[name.lexeme] = std::move(value);
}

int Instance::arity(){
  return 0;
}

std::any Instance::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() == 0 && interpreter.global != nullptr){
    std::cout << "<call from class instance>\n";
  }
  return {};
}


