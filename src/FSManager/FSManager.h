//
// Created by Nick Place on 1/20/24.
//

#ifndef MY_KEY_VALUE_DB_FSMANAGER_H
#define MY_KEY_VALUE_DB_FSMANAGER_H
#include <string>
#include <filesystem>

class FSManager {
private:
    static std::string getHomeDir() {
        char* homeDir;
        #ifdef _WIN32
            homeDir = getenv("USERPROFILE");
        #else
            homeDir = getenv("HOME");
        #endif

        return std::string(homeDir);
    }
public:
    static bool createBaseDirectory();
    inline static const std::string baseDir = getHomeDir() + "/.kv_db";

};


#endif //MY_KEY_VALUE_DB_FSMANAGER_H
