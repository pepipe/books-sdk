cmake_minimum_required(VERSION 3.16)
project(BooksSDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Java and JNI
find_package(Java REQUIRED COMPONENTS Development)
find_package(JNI REQUIRED)

# Include JNI headers
include_directories(${JNI_INCLUDE_DIRS})

# Optional: Print found paths for debugging
message(STATUS "Found Java at: ${Java_JAVA_HOME}")
message(STATUS "JNI include dirs: ${JNI_INCLUDE_DIRS}")
message(STATUS "JNI libraries: ${JNI_LIBRARIES}")

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the BooksSDK library
# add_library(BooksSDK SHARED ${SOURCES})

# Define the BooksSDK as executable
add_executable(BooksSDK "main.cpp" ${SOURCES})

# If executable, then need to copy files
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

# Add include directories after the target is defined
target_include_directories(BooksSDK PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Link external libraries (Zlib, libcurl, etc.)
target_link_libraries(BooksSDK PRIVATE
        ${CMAKE_SOURCE_DIR}/external/lib/zlib.lib
        ${CMAKE_SOURCE_DIR}/external/lib/libcurl.lib
)

# Optional: Output messages to debug paths
message(STATUS "Zlib include: ${CMAKE_SOURCE_DIR}/external/include")
message(STATUS "Zlib library: ${CMAKE_SOURCE_DIR}/external/lib/zlib.lib")

# Tests
# Fetch GoogleTest from the official repository
include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(gtest)

enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(BooksSDKTests ${TEST_SOURCES})

# when BooksSDK is a library
# target_link_libraries(BooksSDKTests PRIVATE BooksSDK gtest gtest_main)

# Enable CTest for testing
add_test(NAME BooksSDKTests COMMAND BooksSDKTests)