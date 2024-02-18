//
// Created by Nick Place on 1/15/24.
//

#ifndef MY_KEY_VALUE_DB_KVDATABASE_H
#define MY_KEY_VALUE_DB_KVDATABASE_H
#include <string>
#include <any>
#include <fstream>
//#include "../variants/ValueTypeVariant.h"
#include "../structs/ValueWithTypeObject.h"
#include "key_value_types.pb.h"




class Value;

class KVDatabase {
private:
    std::string name;
    std::string directoryPath;
    KVMap hashMap;
    std::string storeFilePath;
public:
    KVDatabase(std::string &dbName);
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
    void loadStoreFileIntoHashmap();


    ValueTypeVariant store(std::string &key, const ValueTypeVariant &value, std::string &type);
    ValueTypeVariant get(std::string &key);
    std::string getFilePath(std::string &key);

    void saveToDisk();
    static keyvaluetypes::KeyValueMap* generateProtobufKVMap(const KVMap &hashMap);
    static keyvaluetypes::Value* serializeValue(const ValueTypeVariant &value, const std::string &type);
};


#endif //MY_KEY_VALUE_DB_KVDATABASE_H
