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
        const char* homeDir;
#ifdef _WIN32
        homeDir = getenv("USERPROFILE");
#else
        homeDir = getenv("HOME");
#endif
        if (homeDir == nullptr) {
            throw std::runtime_error("HOME environment variable not set");
        }
        return std::string(homeDir);
    }
    inline static const std::string baseDir = getHomeDir() + "/.kv_db";
    static bool createBaseDirectory();
    static std::string createDbDirectory(const std::string &dbName);
    static std::string createDbStoreFile(const std::string &dbName);
    static std::ofstream openDBWriteStream(const std::string &filePath);
    static std::string readFileContents(const std::string &filePath);
    static std::string getDbPath(const std::string &dbName);
    static std::string getDbStoreFilePath(const std::string &dbName);
    static void appendTimeStampToFileName(std::string &originalPath);
};


#endif //MY_KEY_VALUE_DB_FSMANAGER_H
