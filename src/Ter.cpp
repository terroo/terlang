#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

#include "Ter.hpp"
#include "utils/Debug.hpp"
#include "tokenizer/Scanner.hpp"
#include "parser/Parser.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/Resolver.hpp"

namespace fs = std::filesystem;

Interpreter interpreter{};

void Ter::run_file(const std::string& path){

  Debug::filename = path;

  if(!fs::exists(path)){
    std::cerr << "File not found.\n";
    std::exit(66);
  }

  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if(!file){
    std::cerr << "Permission error opening file.\n";
    std::exit(66);
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> buffer(static_cast<size_t>(size));

  if(!file.read(buffer.data(), size)){
    std::cerr << "Error reading file.\n";
    std::exit(77);
  }

  std::string content(buffer.begin(), buffer.end());
  run(content);
  if(Debug::hadError){ std::exit(65); }
  if(Debug::hadRuntimeError){ std::exit(70); }
}

void Ter::repl(){
  std::string line;
  std::cout << "ter> ";
  for(;;){
    if(!std::getline(std::cin, line) || line == "exit"){
      break;
    }
    run(line);
    if(Debug::hadError){ std::exit(65); }
    if(Debug::hadRuntimeError){ std::exit(70); }
    std::cout << "ter> ";
  }
}

void Ter::run_script(const std::string& script){
  run(script);
  if(Debug::hadError){ std::exit(65); }
  if(Debug::hadRuntimeError){ std::exit(70); }
}

void Ter::run(const std::string& source){
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();
  if(Debug::hadError){ return; }
  Debug::filename = source;

  //Parser parser{tokens};
  //std::vector<std::shared_ptr<Statement::Stmt>> statements = parser.parse();
  auto parser = std::make_unique<Parser>(tokens);

  std::vector<std::shared_ptr<Statement::Stmt>> statements = parser->parse();
  if(Debug::hadError){ return; }

  Resolver resolver{interpreter};
  resolver.resolve(statements);
  if(Debug::hadError){ return; }

  interpreter.interpret(statements);
  if(Debug::hadError){ return; }
}
