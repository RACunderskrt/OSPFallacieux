#include <iostream>
#include <string>

#include "config.h"

using namespace std;

int main(int argc, char *argv[]){
    int option = Config::checkParameters(argc, argv);
    Config test = Config();
    test.chooseOption(option, argv[2]);
    return 0;
}