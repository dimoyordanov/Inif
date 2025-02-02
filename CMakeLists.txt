cmake_minimum_required(VERSION 3.30.5)
project(Inif VERSION 0.1.0 LANGUAGES C CXX)

# Add a library for the example classes
add_library(Inif 
    main.cpp
)

include(CTest)
enable_testing()
#############################################
# Unit tests

# Use FetchContent to get GoogleTest automatically
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2  # You can adjust the version if needed
)

# Make sure GoogleTest is built before tests
FetchContent_MakeAvailable(googletest)
enable_testing()

# Add a testing executable
add_executable(unit_tests 
    unit_tests.cpp 
)

target_link_libraries(unit_tests
    Inif
    gtest_main
)

# Enable GoogleTest testing framework
include(GoogleTest)
gtest_discover_tests(unit_tests)

# Add test for CTest
add_test(NAME test_all COMMAND unit_tests)