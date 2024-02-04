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
    static std::string createDbDirectory(std::string &dbName);
    static std::string createDbStoreFile(std::string &dbName);
    static std::ofstream openDBWriteStream(std::string &filePath);
    static std::string readFileContents(std::string &filePath);
    static std::string getDbPath(std::string &dbName);
    static std::string getDbStoreFilePath(std::string &dbName);
    static void appendTimeStampToFileName(std::string &originalPath);
};


#endif //MY_KEY_VALUE_DB_FSMANAGER_H
