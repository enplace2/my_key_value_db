//
// Created by Nick Place on 1/21/24.
//

#include "KVCli.h"
#include <iostream>
#include "cxxopts.hpp"
#include "../KVDatabase/KVDatabase.h"
#include "CommandParser.h"
#include "../utils/TypeConverter.h"

cxxopts::Options options("kv-db", "CLI for key value db");


int KVCli::handle(int argc, char* argv[]) {
    options.add_options()
            ("c,create", "Create a database")
            ("d,destroy", "Delete a database")
            ("s, store", "store a value")
            ("g, get", "retrieve a value")
            ("n, name", "Database Name (required)", cxxopts::value<std::string>())
            ("k, key", "Key to set or retrieve", cxxopts::value<std::string>())
            ("v, value", "value to set", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);
    if(result.count("n") !=1){
        std::cout << "You must provide a database name with -n <name>" << std::endl;
        return 1;
    }
    std::size_t cCount = result.count("c");
    std::size_t dCount = result.count("d");
    std::size_t sCount = result.count("s");
    std::size_t gCount = result.count("g");

    if((cCount + dCount + sCount + gCount) >1){
        std::cout << "Method conflict: only one of get, store, destroy, create can be called at the same time" << std::endl;
        return 1;
    }
    if(result.count("c")==1){
        std::string dbName = result["n"].as<std::string>();
        KVDatabase::createEmptyDb(dbName);
    }
     if(result.count("d")==1){
        std::string dbName = result["n"].as<std::string>();
       KVDatabase db = KVDatabase::load(dbName);
       db.destroy();
    }
     if(result.count("s")==1){
         if(result.count("k") !=1 || result.count("v")!=1){
             std::cout << "You must provide a key and a value to store" << std::endl;
             return 1;
         }
         std::string dbName = result["n"].as<std::string>();
         std::string key = result["k"].as<std::string>();
         std::string value = result["v"].as<std::string>();
         KVDatabase db = KVDatabase::load(dbName);
         std::string test = "test";
         db.store(key, value, test);
    }
     if(result.count("g")==1){
         if(result.count("k") !=1){
             std::cout << "You must provide a key and a key to retrieve" << std::endl;
             return 1;
         }
         std::string dbName = result["n"].as<std::string>();
         std::string key = result["k"].as<std::string>();
         KVDatabase db = KVDatabase::load(dbName);
         ValueTypeVariant value = db.get(key);
         //std::string valueString = std::get<std::string>(value);
         //std::cout << valueString << std::endl;
    }

    return 0;
}

void KVCli::runInteractiveMode(KVDatabase& db, std::istream& input, std::ostream& output) {
    output << "KVDB CLI is running. Type 'help' for commands or 'exit' to quit." << std::endl;

    std::string line;
    while (true) {
        output << "> ";
        if (!std::getline(input, line)) {
            break;
        }

        ParsedCommand cmd = CommandParser::parse(line);

        if (!cmd.errorMsg.empty()) {
            output << "Error: " << cmd.errorMsg << std::endl;
            continue;
        }

        try {
            switch (cmd.type) {
                case CommandType::SET: {
                    ValueTypeVariant convertedValue;
                    if (cmd.valueType == "string") {
                        convertedValue = cmd.value;
                    } else if (cmd.valueType == "int") {
                        convertedValue = TypeConverter::toInt64(cmd.value);
                    } else if (cmd.valueType == "bool") {
                        convertedValue = TypeConverter::toBool(cmd.value);
                    } else if (cmd.valueType == "double") {
                        convertedValue = TypeConverter::toDouble(cmd.value);
                    } else if (cmd.valueType == "uint") {
                        convertedValue = TypeConverter::toUInt64(cmd.value);
                    } else {
                        output << "Error: Unknown type: " << cmd.valueType << std::endl;
                        break;
                    }
                    db.store(cmd.key, convertedValue, cmd.valueType);
                    output << "Stored: " << cmd.key << " = " << cmd.value << " (" << cmd.valueType << ")" << std::endl;
                    break;
                }

                case CommandType::GET: {
                    ValueTypeVariant value = db.get(cmd.key);
                    std::string valueStr = TypeConverter::variantToString(value);
                    output << valueStr << std::endl;
                    break;
                }

                case CommandType::DELETE: {
                    db.deleteKey(cmd.key);
                    output << "Deleted: " << cmd.key << std::endl;
                    break;
                }

                case CommandType::LIST: {
                    const KVMap& entries = db.getAllEntries();
                    if (entries.empty()) {
                        output << "No entries in database." << std::endl;
                    } else {
                        for (const auto& [key, valueObj] : entries) {
                            std::string valueStr = TypeConverter::variantToString(valueObj.value);
                            std::string typeName = TypeConverter::getTypeName(valueObj.value);
                            output << key << ": " << valueStr << " (" << typeName << ")" << std::endl;
                        }
                    }
                    break;
                }

                case CommandType::EXIT:
                    output << "Exiting KVDB CLI." << std::endl;
                    return;

                case CommandType::HELP:
                    output << "Available commands:" << std::endl;
                    output << "  SET <key> <value> <type>  - Store a value (types: string, int, bool, double, uint)" << std::endl;
                    output << "  GET <key>                 - Retrieve a value" << std::endl;
                    output << "  DELETE <key>              - Remove a key" << std::endl;
                    output << "  LIST                      - Show all entries" << std::endl;
                    output << "  EXIT                      - Quit the CLI" << std::endl;
                    output << "  HELP                      - Show this message" << std::endl;
                    break;

                case CommandType::INVALID:
                    output << "Invalid command." << std::endl;
                    break;
            }
        } catch (const std::exception& e) {
            output << "Error: " << e.what() << std::endl;
        }
    }
}
