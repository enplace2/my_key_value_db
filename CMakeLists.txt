cmake_minimum_required(VERSION 3.27)
project(my_key_value_db)

set(CMAKE_CXX_STANDARD 17)

add_executable(my_key_value_db main.cpp
        src/KVDatabase/KVDatabase.cpp
        src/KVDatabase/KVDatabase.h
        src/FSManager/FSManager.cpp
        src/FSManager/FSManager.h
        src/CLI/cxxopts.hpp
        src/CLI/KVCli.cpp
        src/CLI/KVCli.h
        src/variants/ValueTypeVariant.h
        src/structs/ValueWithTypeObject.h
)
add_subdirectory(Google_tests)
enable_testing()

add_test(NAME Google_Tests COMMAND Google_Tests_run)

