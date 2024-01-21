//
// Created by Nick Place on 1/15/24.
//

#ifndef MY_KEY_VALUE_DB_KVDATABASE_H
#define MY_KEY_VALUE_DB_KVDATABASE_H
#include <string>
#include <any>
#include <fstream>


class KVDatabase {
private:
    std::string name;
    std::string fullPath;
public:
    KVDatabase(const std::string& dbName, const std::string& fullPath);
    ~KVDatabase();

    /*-----------------------------------------
     * GETTER METHODS
     * ---------------------------------------*/
    std::string getName() const {
        return this->name;
    }
    std::string getPath() const {
        return this->fullPath;
    };
    std::string getDirectory();
    /*-----------------------------------------
     * DB METHODS
     * ---------------------------------------*/
    static KVDatabase createEmptyDb(std::string& dbName);
    void destroy();
    static KVDatabase load(std::string &dbName);


    std::string store(std::string &key, std::string &value);
    std::string get(std::string &key);
    std::string getFilePath(std::string &key);
};


#endif //MY_KEY_VALUE_DB_KVDATABASE_H
