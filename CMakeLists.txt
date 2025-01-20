cmake_minimum_required(VERSION 3.16)
project(GoogleBooksSDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include external headers
include_directories(${CMAKE_SOURCE_DIR}/external/include)

# Link external libraries
link_directories(${CMAKE_SOURCE_DIR}/external/lib)

# Add source files
add_executable(GoogleBooksSDK "main.cpp")

# Link libcurl
target_link_libraries(GoogleBooksSDK curl)

# Define the external/bin directory
set(EXTERNAL_BIN_DIR ${CMAKE_SOURCE_DIR}/external/bin)

# Define the target output directory (build directory)
set(TARGET_OUTPUT_DIR ${CMAKE_BINARY_DIR})

# Add a custom post-build command to copy files
add_custom_command(TARGET GoogleBooksSDK POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${EXTERNAL_BIN_DIR} ${TARGET_OUTPUT_DIR}
        COMMENT "Copying external DLLs from ${EXTERNAL_BIN_DIR} to ${TARGET_OUTPUT_DIR}"
)