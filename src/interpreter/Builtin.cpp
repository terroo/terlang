#include "Builtin.hpp"
#include "ArrayType.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include "../utils/Helpers.hpp"

void builtinError(const std::string& nameBuiltin){
    std::cerr << "Builtin '" << nameBuiltin << "' function error.\n";
    std::exit(1);
}

// ------ Clock -----------
int Clock::arity(){
  return 0;
}

std::any Clock::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() > (size_t)arity() && interpreter.global != nullptr){
    builtinError("clock");
  }
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  double c = static_cast<double>(millis) / 1000.0;
  return std::any_cast<double>(c);
}

std::string Clock::toString(){
  return "<function builtin>";
}

// ------ RAND -----------
int Rand::arity(){
  return 2;
}

std::any Rand::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() != (size_t)arity() && interpreter.global != nullptr){
    builtinError("rand");
  }

  for(size_t i = 0; i < (size_t)arity(); i++){
    if(!arguments[i].has_value() || (arguments[i].type() != typeid(double))){
      builtinError("rand");
    }
  }

  double a = std::any_cast<double>(arguments[0]);
  double b = std::any_cast<double>(arguments[1]);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dist(static_cast<int>(a), static_cast<int>(b));

  double random_number = dist(gen);

  return std::any_cast<double>(random_number);
}

std::string Rand::toString(){
  return "<function builtin>";
}

// ------ GetEnv -----------
int GetEnv::arity(){
  return 1;
}

std::any GetEnv::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() > (size_t)arity() && interpreter.global != nullptr){
    builtinError("getenv");
  }

  std::string name = std::any_cast<std::string>(arguments[0]);
  std::string envname = std::getenv(name.data());

  return std::any_cast<std::string>(envname);
}

std::string GetEnv::toString(){
  return "<function builtin>";
}

// ------ ToString -----------
int ToString::arity(){
  return 1;
}

std::any ToString::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() > (size_t)arity() && interpreter.global != nullptr){
    builtinError("to_string");
  }

  if(!arguments[0].has_value() || arguments[0].type() != typeid(double)){
    builtinError("to_string");
  }

  int terint = static_cast<int>(std::any_cast<double>(arguments[0]));
  std::string str = std::to_string(std::any_cast<int>(terint));

  return std::any_cast<std::string>(str);
}

std::string ToString::toString(){
  return "<function builtin>";
}

// ------ Args -----------
int Args::arity(){
  return 0;
}

std::any Args::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() > (size_t)arity() && interpreter.global != nullptr){
    builtinError("args");
  }

  std::vector<std::string> args = Helpers::get_instance().get_args();
  auto arr = std::make_shared<ArrayType>();

  for(size_t i = 0; i < args.size(); ++i){
    if(i != 0 && i != 1){
      arr->append(args[i]);
    }
  }

  return std::any(arr);
}

std::string Args::toString(){
  return "<function builtin>";
}

// ------ Exec -----------
int Exec::arity(){
  return 1;
}

std::any Exec::call(Interpreter &interpreter, std::vector<std::any> arguments){
  if(arguments.size() > (size_t)arity() && interpreter.global != nullptr){
    builtinError("exec");
  }

  std::string name = std::any_cast<std::string>(arguments[0]);
  int run = std::system(name.data());
  
  if(run != 0){
    builtinError("exec[system]");
  }

  std::string str_run = {};

  return std::any_cast<std::string>(str_run);
}

std::string Exec::toString(){
  return "<function builtin>";
}

// ------ Input -----------
int Input::arity() {
  return 0;
}

std::any Input::call(Interpreter &interpreter, std::vector<std::any> arguments) {
  if (arguments.size() > (size_t)arity() && interpreter.global != nullptr) {
    builtinError("input");
  }

  std::string input;
  std::getline(std::cin, input);

  return std::any(input);
}

std::string Input::toString() {
  return "<function builtin>";
}
