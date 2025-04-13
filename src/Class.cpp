#include "Class.hpp"
#include "Instance.hpp"

Class::Class(const std::string& name, std::unordered_map<std::string, std::shared_ptr<Function>> methods) :
  name(name), methods(methods) {}

std::any Class::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() == 0 && interpreter.global != nullptr){
    std::cout << "<call from class>\n";
  }

  auto instance = std::make_shared<Instance>(std::make_shared<Class>(*this));
  return instance;
}

std::string Class::toString(){
  return "<class " + this->name + ">";
}

int Class::arity(){
  return 0;
}

std::optional<std::shared_ptr<Function>> Class::findMethod(const std::string& l_name) {
  auto it = methods.find(l_name);
  if(it != methods.end()){
    return it->second;
  }
  return std::nullopt;
}
