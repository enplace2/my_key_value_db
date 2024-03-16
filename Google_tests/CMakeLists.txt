project(Google_tests)

# Add the Google Test subdirectory
add_subdirectory(lib/googletest)

# Directly specify the path to the protobuf source and header files
set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/../protos/key_value_types.pb.cc")
set(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/../protos/key_value_types.pb.h")


# Create a test executable
add_executable(Google_Tests_run test1.cpp
        ../src/KVDatabase/KVDatabase.h
        ../src/KVDatabase/KVDatabase.cpp
        ../src/FSManager/FSManager.cpp
        ../src/FSManager/FSManager.h
        ../src/CLI/KVCli.cpp
        ../src/CLI/KVCli.h
        ${PROTO_SRC}
        ${PROTO_HDR}
        ../src/Parsers/JSONParser/Enums/TokenType.h
)

# Include the Google Test headers
target_include_directories(Google_Tests_run PRIVATE lib/googletest/googletest/include)

target_include_directories(Google_Tests_run PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../protos)


# Link the Google Test libraries
target_link_libraries(Google_Tests_run gtest gtest_main protobuf::libprotobuf)

