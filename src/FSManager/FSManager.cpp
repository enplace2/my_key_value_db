//
// Created by Nick Place on 1/20/24.
//

#include "FSManager.h"
#include <filesystem>
namespace fs = std::filesystem;

bool FSManager::createBaseDirectory() {
    if(!fs::exists(FSManager::baseDir)){
       return fs::create_directory(FSManager::baseDir);
    }
    return fs::exists(FSManager::baseDir);
}
