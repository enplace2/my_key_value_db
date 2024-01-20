//
// Created by Nick Place on 1/15/24.
//

#include "KVDatabase.h"
#include "../FSManager/FSManager.h"


KVDatabase::KVDatabase(const std::string& dbName, const std::string& fullPath) {
    this->name = dbName;
    this->fullPath = fullPath;
}

KVDatabase::~KVDatabase()  = default;

KVDatabase KVDatabase::createEmptyDb(std::string &dbName) {

    return KVDatabase{dbName, dbName};
}

std::string KVDatabase::getDirectory(void) {
    return std::string();
}
