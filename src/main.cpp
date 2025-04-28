#include <iostream>

#include "Ter.hpp"
#include "utils/Helpers.hpp"

void help(const std::string& prog){
  std::cerr << "Ter/Terlang v0.1.1\n\n";
  std::cerr << "Usage: \n\t" <<
    prog << " [filename].ter\n\t" << 
    prog << " -e '<script>'\n";
}

int main(int argc, char **argv){

  if(argc >= 2){
    std::vector<std::string> args;
    for(int i = 0; i < argc; ++i){
      args.emplace_back(argv[i]);
    }

    Helpers::get_instance().set_args(argc, args);

    const std::string arg1 = argv[1];

    if(arg1 == "-e"){
      if(argc != 3 || std::string(argv[2]).empty()) {
        std::cerr << "Error: Missing script argument after -e\n";
        return EXIT_FAILURE;
      }
      Ter::run_script(argv[2]);
      return EXIT_SUCCESS;
    }

    const std::string filename = argv[1];
    const std::string hext = "\x2e\x74\x65\x72";

    if(filename.length() >= 4 && filename.substr(filename.length() - 4) == hext){
      Ter::run_file(argv[1]);
      return EXIT_SUCCESS;
    }

    help(argv[0]);
    return EXIT_FAILURE;
  }

  Ter::repl();
  return EXIT_SUCCESS;
}

