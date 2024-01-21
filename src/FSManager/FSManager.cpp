//
// Created by Nick Place on 1/20/24.
//

#include "FSManager.h"
#include <filesystem>
#include <fstream>
#include <string>
namespace fs = std::filesystem;

bool FSManager::createBaseDirectory() {
    if(!fs::exists(FSManager::baseDir)){
       return fs::create_directory(FSManager::baseDir);
    }
    return fs::exists(FSManager::baseDir);
}

std::string FSManager::createDBDirectory(std::string &dbName) {
    std::string dbDirectoryPath = getDbPath(dbName);

    if(!fs::exists(dbDirectoryPath)){
        fs::create_directory(dbDirectoryPath);
        if(!fs::exists(dbDirectoryPath)) return "";
        return dbDirectoryPath;
    }
    return dbDirectoryPath;
}

std::ofstream FSManager::openDBWriteStream(std::string &directoryPath, std::string &key) {
    std::ofstream os;
    os.open(directoryPath + "/" + key + "_string.kv");
    return os;
}

std::string FSManager::readFileContents(std::string &filePath) {
    std::ifstream is(filePath);
    if(is){
        std::string content((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        return content;
    }
    return "";
}

std::string FSManager::getDbPath(std::string &dbName) {
    std::string dbDirectoryPath = FSManager::baseDir + "/" +dbName;
    return dbDirectoryPath;
}
