//
// Created by Nick Place on 1/20/24.
//

#ifndef MY_KEY_VALUE_DB_FSMANAGER_H
#define MY_KEY_VALUE_DB_FSMANAGER_H
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
class FSManager {
private:

public:
    static std::string getHomeDir() {
        char* homeDir;
#ifdef _WIN32
        homeDir = getenv("USERPROFILE");
#else
        homeDir = getenv("HOME");
#endif

        return std::string(homeDir);
    }
    inline static const std::string baseDir = getHomeDir() + "/.kv_db";
    static bool createBaseDirectory();
    static std::string createDBDirectory(std::string &dbName);
    static std::ofstream openDBWriteStream(std::string &directoryPath, std::string &key);
    static std::string readFileContents(std::string &filePath);
    static std::string getDbPath(std::string &dbName);
};


#endif //MY_KEY_VALUE_DB_FSMANAGER_H
