cmake_minimum_required(VERSION 3.30)
project(GoogleBooksSDK)

set(CMAKE_CXX_STANDARD 17)

# Include paths
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link libraries
find_package(CURL REQUIRED)
target_link_libraries(GoogleBooksAPI CURL::libcurl)

# JSON for Modern C++
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(external/json)
target_link_libraries(GoogleBooksAPI nlohmann_json::nlohmann_json)

# SQLite (optional)
find_package(SQLite3 REQUIRED)
target_link_libraries(GoogleBooksAPI SQLite::SQLite3)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(GoogleBooksSDK ${SOURCES})
