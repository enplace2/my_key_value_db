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

std::string FSManager::createDbDirectory(std::string &dbName) {
    std::string dbDirectoryPath = getDbPath(dbName);

    if(!fs::exists(dbDirectoryPath)){
        fs::create_directory(dbDirectoryPath);
        if(!fs::exists(dbDirectoryPath)) return "";
        return dbDirectoryPath;
    }
    return dbDirectoryPath;
}

std::string FSManager::createDbStoreFile(std::string &dbName) {
    std::string dbStoreFilePath = getDbStoreFilePath(dbName);

    // Open and immediately close an ofstream with the path.
    // This creates an empty file.
    std::ofstream file(dbStoreFilePath);

    if (!file) {
        std::cerr << "Failed to create file at: " << dbStoreFilePath << std::endl;
    }

    // Return the file path
    return dbStoreFilePath;
}


std::ofstream FSManager::openDBWriteStream(std::string &filePath) {
    std::ofstream os;
    os.open(filePath);
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

std::string FSManager::getDbStoreFilePath(std::string &dbName) {
    std::string dbStoreFilePath = getDbPath(dbName) + "/" +"store.kv";
    return dbStoreFilePath;
}

void FSManager::appendTimeStampToFileName(std::string &originalPath) {
    if (std::filesystem::exists(originalPath)) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_c), "%Y%m%d%H%M%S");

        std::filesystem::path pathObj(originalPath);
        std::string newName = pathObj.parent_path().string() + "/" + pathObj.stem().string() + "_" + ss.str() + pathObj.extension().string();

        std::filesystem::rename(originalPath, newName); // Rename the file
    }
}
