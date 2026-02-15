cmake_minimum_required(VERSION 3.27)
project(my_key_value_db)

set(CMAKE_CXX_STANDARD 17)

add_executable(my_key_value_db
        main.cpp
        src/KVDatabase/KVDatabase.cpp
        src/KVDatabase/KVDatabase.h
        src/FSManager/FSManager.cpp
        src/FSManager/FSManager.h
        src/CLI/cxxopts.hpp
        src/CLI/KVCli.cpp
        src/CLI/KVCli.h
        #src/variants/ValueTypeVariant.h
        src/structs/ValueWithTypeObject.h
        src/Parsers/JSONParser/Enums/TokenType.h
        src/Parsers/JSONParser/Structs/Token.h
        src/Parsers/JSONParser/Tokenizer/JSONTokenizer.cpp
        src/Parsers/JSONParser/Tokenizer/JSONTokenizer.h
)


#---------------------------------------------------------
# Protobuf configuration
#---------------------------------------------------------

# Find the Protobuf package
find_package(protobuf REQUIRED CONFIG)

# Include the CMake binary directory for accessing generated files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Specify where the protobuf files are
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/protos/*.proto")

# Generate C++ sources and headers from the .proto files
protobuf_generate(TARGET my_key_value_db LANGUAGE cpp PROTOS ${PROTO_FILES})

# After protobuf_generate command
target_include_directories(my_key_value_db PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/protos)

# Link with the Protobuf library
target_link_libraries(my_key_value_db protobuf::libprotobuf)


#---------------------------------------------------------
# Google Tests
#---------------------------------------------------------
enable_testing()
add_subdirectory(Google_tests)
add_test(NAME Google_Tests COMMAND Google_Tests_run)






