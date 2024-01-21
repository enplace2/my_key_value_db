project(Google_tests)

# Add the Google Test subdirectory
add_subdirectory(lib/googletest)

# Include the Google Test headers
include_directories(lib/googletest/googletest/include)

# Create a test executable
add_executable(Google_Tests_run test1.cpp
        ../src/KVDatabase/KVDatabase.h
        ../src/KVDatabase/KVDatabase.cpp
        ../src/FSManager/FSManager.cpp
        ../src/FSManager/FSManager.h
        ../src/CLI/KVCli.cpp
        ../src/CLI/KVCli.h
)
target_link_libraries(Google_Tests_run gtest gtest_main)
