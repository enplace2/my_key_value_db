//
// Created by Nick Place on 1/15/24.
//

#include "KVDatabase.h"
#include "../FSManager/FSManager.h"
#include <filesystem>

namespace fs = std::filesystem;

KVDatabase::KVDatabase(const std::string& dbName, const std::string& directoryPath) {
    this->name = dbName;
    this->directoryPath = directoryPath;
}

KVDatabase::~KVDatabase()  = default;

std::string KVDatabase::getDirectory() {
    return this->directoryPath;
}
KVDatabase KVDatabase::createEmptyDb(std::string &dbName) {
    std::string directoryPath = FSManager::createDBDirectory(dbName);
    return KVDatabase(dbName, directoryPath);
}

void KVDatabase::destroy() {
    if(fs::exists(this->directoryPath)){
        fs::remove_all(this->directoryPath);
    }
}

ValueTypeVariant KVDatabase::store(std::string &key, const ValueTypeVariant &value, std::string &type) {
    this->hashMap[key] = {value, type};
    auto& valueWithTypeInfo = this->hashMap[key];
    auto& savedValue = valueWithTypeInfo.value;
    saveToDisk();
    return savedValue;
}

std::string KVDatabase::get(std::string &key){
    std::string filePath = getFilePath(key);
    std::string value = FSManager::readFileContents(filePath);
    return value;
}

std::string KVDatabase::getFilePath(std::string &key) {
    return this->directoryPath + "/" + key + "_string.kv";
}

KVDatabase KVDatabase::load(std::string &dbName) {
    std::string dbDirectoryPath = FSManager::getDbPath(dbName);
    return KVDatabase(dbName, dbDirectoryPath);
}


void KVDatabase::saveToDisk() {
    //get the file path to save to
    std::string dbStoreFilePath = FSManager::getDbStoreFilePath(this->name);
    //if a store file already exists, version it by appending a timestamp
    FSManager::appendTimeStampToFileName(dbStoreFilePath);

    //@TODO: should delete versions regularly after some time period



    // write the new file to disk
    //delete previous file?



    //check if file exists
    //if so, append a timestamp to the name, before the extension

}
