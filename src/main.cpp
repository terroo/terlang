#include "Ter.hpp"

void help(const std::string& prog){
  std::cerr << "Usage: " << prog << " [filename].ter\n";
}

int main(int argc, char **argv){

  if(argc > 2){

    const std::string e = argv[1];

    if(e == "-e"){
      if (argc > 3 || argv[2] == nullptr || std::string(argv[2]).empty()) {
        std::cerr << "Error: Missing script argument after -e\n";
        return EXIT_FAILURE;
      }
      Ter::run_script(argv[2]);
      return EXIT_SUCCESS;
    }

    help(argv[0]);
    return EXIT_FAILURE;
  }

  if(argc == 2){
    const std::string filename = argv[1];
    const std::string hext = "\x2e\x74\x65\x72";
    if(filename.substr( filename.length() - 4, 4 ) != hext){
      help(argv[0]);
      return EXIT_FAILURE;
    }
    Ter::run_file(argv[1]);
  }else{
    Ter::repl();
  }

  return EXIT_SUCCESS;
}

