//
// Created by Nick Place on 1/15/24.
//

#ifndef MY_KEY_VALUE_DB_KVDATABASE_H
#define MY_KEY_VALUE_DB_KVDATABASE_H
#include <string>


class KVDatabase {
private:
    std::string name;
    std::string fullPath;
public:
    KVDatabase(const std::string& dbName, const std::string& fullPath);
    ~KVDatabase();
    static KVDatabase createEmptyDb(std::string& dbName);
    std::string getName() const {
        return this->name;
    }
    std::string getPath() const {
        return this->fullPath;
    };
    std::string getDirectory();
};


#endif //MY_KEY_VALUE_DB_KVDATABASE_H
