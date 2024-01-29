//
// Created by Nick Place on 1/15/24.
//

#ifndef MY_KEY_VALUE_DB_KVDATABASE_H
#define MY_KEY_VALUE_DB_KVDATABASE_H
#include <string>
#include <any>
#include <fstream>
#include "../variants/ValueTypeVariant.h"
#include "../structs/ValueWithTypeObject.h"

// Now you can use KeyValueVariant here




class KVDatabase {
private:
    std::string name;
    std::string directoryPath;
    std::unordered_map<std::string, ValueWithTypeObject> hashMap;
public:
    KVDatabase(const std::string& dbName, const std::string& directoryPath);
    ~KVDatabase();

    /*-----------------------------------------
     * GETTER METHODS
     * ---------------------------------------*/
    std::string getName() const {
        return this->name;
    }
    std::string getPath() const {
        return this->directoryPath;
    };
    std::string getDirectory();
    /*-----------------------------------------
     * DB METHODS
     * ---------------------------------------*/
    static KVDatabase createEmptyDb(std::string& dbName);
    void destroy();
    static KVDatabase load(std::string &dbName);


    ValueTypeVariant store(std::string &key, const ValueTypeVariant &value, std::string &type);
    std::string get(std::string &key);
    std::string getFilePath(std::string &key);

    void saveToDisk();
};


#endif //MY_KEY_VALUE_DB_KVDATABASE_H
