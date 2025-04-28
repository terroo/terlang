#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "IncludeRun.hpp"
#include "../utils/Debug.hpp"
#include "../tokenizer/Scanner.hpp"

namespace fs = std::filesystem;

std::vector<Token> IncludeRun::tokens;

void IncludeRun::scanFile(std::string path){
  path.erase(remove( path.begin(), path.end(), '\"' ),path.end());
  Debug::filename = path;

  if(!fs::exists(path)){
    std::cerr << "File '" << path << "' not found.\n";
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

void IncludeRun::run(const std::string& source){
  Scanner scanner(source);
  IncludeRun::tokens = scanner.scanTokens();
  if(Debug::hadError){ return; }
}


std::vector<Token> IncludeRun::getTokens(){
  return IncludeRun::tokens;
}
