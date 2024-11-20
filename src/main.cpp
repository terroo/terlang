#include "Ter.hpp"

void help(const std::string& prog){
  std::cerr << "Usage: " << prog << " [filename].ter\n";
}

int main(int argc, char **argv){
  if(argc > 2){
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

