#include "../include/parser.h"

std::vector<std::string> reader_file(char* filename){

  std::vector<std::string> expressions;

  std::ifstream file (filename);
  if(file.is_open()){

    std::string str;
    while (std::getline(file, str))
    {
        expressions.push_back(str);
    }
  }else{
    std::cout<< "Wrong syntaxe, add the path of a file containing the expressions to be analyzed!\n";
    std::exit(0);
  }

  return expressions;
}
