//
// Created by Nick Place on 1/15/24.
//

#include "KVDatabase.h"
#include "../FSManager/FSManager.h"
#include "key_value_types.pb.h"



#include <filesystem>

namespace fs = std::filesystem;

KVDatabase::KVDatabase(std::string &dbName) {
    std::string dbDirectoryPath = FSManager::getDbPath(dbName);
    std::string dbStoreFilePath = FSManager::getDbStoreFilePath(dbName);
    this->name = dbName;
    this->directoryPath = dbDirectoryPath;
    this->storeFilePath = dbStoreFilePath;
    loadStoreFileIntoHashmap();
}

KVDatabase::~KVDatabase()  = default;

std::string KVDatabase::getDirectory() {
    return this->directoryPath;
}
KVDatabase KVDatabase:: createEmptyDb(std::string &dbName) {
    std::string directoryPath = FSManager::createDbDirectory(dbName);
    std::string dbStoreFile = FSManager::createDbStoreFile(dbName);
    return KVDatabase(dbName);
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

ValueTypeVariant KVDatabase::get(std::string &key){
    std::string filePath = getFilePath(key);
    const auto [value, type] = this->hashMap[key];
    return value;
}

std::string KVDatabase::getFilePath(std::string &key) {
    return this->directoryPath + "/" + key + "_string.kv";
}

KVDatabase KVDatabase::load(std::string &dbName) {
    std::string dbDirectoryPath = FSManager::getDbPath(dbName);
    return KVDatabase(dbName);
}


void KVDatabase::saveToDisk() {
    //get the file path to save to
    std::string dbStoreFilePath = FSManager::getDbStoreFilePath(this->name);
    //if a store file already exists, version it by appending a timestamp
    FSManager::appendTimeStampToFileName(dbStoreFilePath);
    keyvaluetypes::KeyValueMap kvMap = generateProtobufKVMap(this->hashMap);
    // write the serialized data to a file at dbStoreFilePath
    std::string serializedData;
    kvMap.SerializeToString(&serializedData);
    std::ofstream outputFile(dbStoreFilePath, std::ios::binary);
    outputFile.write(serializedData.data(), serializedData.size());

}

keyvaluetypes::KeyValueMap KVDatabase::generateProtobufKVMap(const KVMap &hashMap){
    keyvaluetypes::KeyValueMap kvMap;

    //iterate through this->hashmap
    for(const auto &item : hashMap){
        // each hashmap item has a type that corresponds to one of the keyvaluetypes::KeyValue types.
        // add those to the kvMap
        const auto key = item.first;
        const auto [value, type] = item.second;
        keyvaluetypes::KeyValue* kv = kvMap.add_items();
        kv->set_key(key);
        keyvaluetypes::Value* val = serializeValue(value, type);
        kv->set_allocated_value(val);
    }
    return kvMap;
};

// Correct the function signature to return a pointer and accept const parameters
keyvaluetypes::Value* KVDatabase::serializeValue(const ValueTypeVariant &value, const std::string &type){
    keyvaluetypes::Value* val = new keyvaluetypes::Value();

    if(type == "string"){
        std::string castValue = std::get<std::string>(value);
        val->set_string_value(castValue);
    }else if (type =="int"){
        int64_t castValue = std::get<int64_t>(value);
        val->set_int_value(castValue);
    }else if (type =="bool"){
        bool castValue = std::get<bool>(value);
        val->set_bool_value(castValue);
    }else if (type =="double"){
        double castValue = std::get<double>(value);
        val->set_double_value(castValue);
    }else if (type =="uint"){
        uint64_t castValue = std::get<uint64_t>(value);
        val->set_uint_value(castValue);
    }else if (type == "map"){
        KVMap castValue = std::get<KVMap>(value);
        keyvaluetypes::KeyValueMap nestedKVMap = generateProtobufKVMap(castValue);
        val->set_allocated_map(nestedKVMap);
    }
    return val;
}


void KVDatabase::loadStoreFileIntoHashmap() {
    std::ifstream inputFile(this->storeFilePath, std::ios::binary);

    if (!inputFile) {
        throw std::runtime_error("Error opening file for reading.");
    }

    std::string serializedData((std::istreambuf_iterator<char>(inputFile)),
                               std::istreambuf_iterator<char>());

    keyvaluetypes::KeyValueMap kvMap;
    if (!kvMap.ParseFromString(serializedData)) {
        // Handle error: parsing failed
        throw std::runtime_error("Error parsing file content.");
    }

    for (const auto& item : kvMap.items()) {
        const auto& key = item.key();
        const auto& val = item.value();
        ValueTypeVariant value;
        std::string type;

        if (val.has_string_value()) {
            value = val.string_value();
            type = "string";
        } else if (val.has_int_value()) {
            value = val.int_value();
            type = "int";
        } else if (val.has_bool_value()) {
            value = val.bool_value();
            type = "bool";
        } else if (val.has_double_value()) {
            value = val.double_value();
            type = "double";
        } else if (val.has_uint_value()) {
            value = val.uint_value();
            type = "uint";
        }

        this->hashMap[key] = {value, type};
    }
}
