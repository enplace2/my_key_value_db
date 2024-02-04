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

find_package(Protobuf REQUIRED)

# Include the Protobuf headers
include_directories(${PROTOBUF_INCLUDE_DIRS})

# Link against the Protobuf libraries
target_link_libraries(my_key_value_db ${PROTOBUF_LIBRARIES})

# If you have .proto files in your project, you can use
# PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS your_proto_files.proto)
# add_executable(my_key_value_db ${PROTO_SRCS} ${PROTO_HDRS} other_sources...)


# Specify the directories to include
#[[include_directories(
        external/protobuf/src
        external/protobuf/third_party/abseil-cpp
        # Consider removing or commenting out Google Test includes if not needed
        # external/protobuf/third_party/googletest
        # external/protobuf/third_party/googletest/include/gmock
        external/protobuf/third_party/jsoncpp
        external/protobuf/third_party/lunit
        external/protobuf/third_party/utf8_range
)]]

#[[
# Add the Protobuf source files to your project, excluding tests and benchmarks
file(GLOB PROTOBUF_SOURCES "external/protobuf/src/google/protobuf/*.cc")
list(FILTER PROTOBUF_SOURCES EXCLUDE REGEX ".*tester\\.cc$|.*test\\.cc$|.*test_util\\.cc$|.*benchmark\\.cc$|.*map_probe_benchmark\\.cc$")
list(FILTER PROTOBUF_SOURCES EXCLUDE REGEX ".*_test\\.cc$|.*unittest.*\\.cc$|.*test_util\\.cc$|.*benchmark\\.cc$|.*map_probe_benchmark\\.cc$|.*tester\\.cc$|.*test_util_lite\\.cc$")
]]

#add_library(protobuf ${PROTOBUF_SOURCES})
#target_link_libraries(my_key_value_db protobuf)

# Include the Protobuf headers
# target_include_directories(protobuf PUBLIC external/protobuf/src)

# Optional: Google_tests and testing configurations
# Consider if Google_tests directory is necessary for your project
# add_subdirectory(Google_tests)
# enable_testing()
# add_test(NAME Google_Tests COMMAND Google_Tests_run)
