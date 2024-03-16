#include <iostream>
#include "src/FSManager/FSManager.h"
#include "src/CLI/KVCli.h"
#include "src/KVDatabase/KVDatabase.h"
using namespace std;


int main(int argc, char* argv[]) {
    bool baseDirectoryExists = FSManager::createBaseDirectory();
    if(!baseDirectoryExists) return 1;
    std::string input;

    std::cout << "KVDB CLI is running. Type 'exit' to quit.\n";

    do {
        std::cout << "> "; // Prompt for input
        std::getline(std::cin, input); // Get input from user

        // Process the input here. For now, just echo it back.
        std::cout << "You entered: " << input << std::endl;

        // Later, this is where you'll parse the input with the CLI library
        // and handle different commands accordingly.

    } while (input != "exit"); // Exit condition

    std::cout << "Exiting KVDB CLI.\n";

    return 0;
}
