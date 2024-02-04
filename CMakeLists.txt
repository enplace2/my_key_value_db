cmake_minimum_required(VERSION 3.27)
project(my_key_value_db)

set(CMAKE_CXX_STANDARD 17)

# Find the Protobuf package
find_package(protobuf REQUIRED CONFIG)

# Include the Protobuf headers
include_directories(${PROTOBUF_INCLUDE_DIRS})

# Include generated *.pb.h files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Specify where the protobuf files are
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/protos/*.proto")

# Add the executable target
add_executable(my_key_value_db
        main.cpp
        src/KVDatabase/KVDatabase.cpp
        src/KVDatabase/KVDatabase.h
        src/FSManager/FSManager.cpp
        src/FSManager/FSManager.h
        src/CLI/cxxopts.hpp
        src/CLI/KVCli.cpp
        src/CLI/KVCli.h
        src/variants/ValueTypeVariant.h
        src/structs/ValueWithTypeObject.h
        ${PROTO_SRCS}
        ${PROTO_HDRS}
)

# Link with the Protobuf library
target_link_libraries(my_key_value_db protobuf::libprotobuf)





# Generate C++ sources and headers from the .proto files
protobuf_generate(TARGET my_key_value_db LANGUAGE cpp PROTOS ${PROTO_FILES})


