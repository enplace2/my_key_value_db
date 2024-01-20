# CMake generated Testfile for 
# Source directory: /Users/nickplace/CLionProjects/my_key_value_db
# Build directory: /Users/nickplace/CLionProjects/my_key_value_db/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[Google_Tests]=] "/Users/nickplace/CLionProjects/my_key_value_db/build/Google_tests/Google_Tests_run")
set_tests_properties([=[Google_Tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/nickplace/CLionProjects/my_key_value_db/CMakeLists.txt;13;add_test;/Users/nickplace/CLionProjects/my_key_value_db/CMakeLists.txt;0;")
subdirs("Google_tests")
