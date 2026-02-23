//
// Created by Nick Place on 1/21/24.
//

#ifndef MY_KEY_VALUE_DB_KVCLI_H
#define MY_KEY_VALUE_DB_KVCLI_H


#include "cxxopts.hpp"
#include <iostream>

class KVDatabase;

class KVCli {
private:
public:
    static int handle(int argc, char* argv[]);
    static void runInteractiveMode(KVDatabase& db,
                                   std::istream& input = std::cin,
                                   std::ostream& output = std::cout);

};


#endif //MY_KEY_VALUE_DB_KVCLI_H
