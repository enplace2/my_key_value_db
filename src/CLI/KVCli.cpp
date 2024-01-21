//
// Created by Nick Place on 1/21/24.
//

#include "KVCli.h"
#include <iostream>
#include "cxxopts.hpp"
#include "../KVDatabase/KVDatabase.h"

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
         db.store(key, value);
    }
     if(result.count("g")==1){
         if(result.count("k") !=1){
             std::cout << "You must provide a key and a key to retrieve" << std::endl;
             return 1;
         }
         std::string dbName = result["n"].as<std::string>();
         std::string key = result["k"].as<std::string>();
         KVDatabase db = KVDatabase::load(dbName);
         std::string value = db.get(key);
         std::cout << value << std::endl;
    }

    return 0;
}
