cmake_minimum_required(VERSION 3.16)
project(BooksSDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Include external headers
include_directories(${CMAKE_SOURCE_DIR}/external/include)

# Link external libraries
link_directories(${CMAKE_SOURCE_DIR}/external/lib)

# Fetch GoogleTest from the official repository
include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(gtest)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(BooksSDK "main.cpp" ${SOURCES})

# Link libcurl
target_link_libraries(BooksSDK curl)

# Define the external/bin directory
set(EXTERNAL_BIN_DIR ${CMAKE_SOURCE_DIR}/external/bin)

# Define the target output directory (build directory)
set(TARGET_OUTPUT_DIR ${CMAKE_BINARY_DIR})

# Add a custom post-build command to copy files
add_custom_command(TARGET BooksSDK POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${EXTERNAL_BIN_DIR} ${TARGET_OUTPUT_DIR}
        COMMENT "Copying external DLLs from ${EXTERNAL_BIN_DIR} to ${TARGET_OUTPUT_DIR}"
)

# Enable testing
enable_testing()

# Add test sources
file(GLOB TEST_SOURCES "tests/*.cpp")

# Create a test executable
add_executable(BooksSDKTests ${TEST_SOURCES})

# Link the test executable to GoogleTest
target_link_libraries(BooksSDKTests
        PRIVATE gtest gtest_main
)

# Add the test executable to CTest
add_test(NAME BooksSDKTests COMMAND BooksSDKTests)
