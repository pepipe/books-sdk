cmake_minimum_required(VERSION 3.16)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if(DEFINED ANDROID)
    option(BUILD_FOR_ANDROID "Build project for Android" ON)
else ()
    option(BUILD_FOR_ANDROID "Build project for Android" OFF)
endif ()

if(DEFINED LIB)
    option(BUILD_AS_LIB "Build project as a library" ON)
else ()
    option(BUILD_AS_LIB "Build project as a library" OFF)
endif ()

if(BUILD_FOR_ANDROID)
    include(my-toolchain.cmake)
endif ()

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

if (BUILD_AS_LIB)
    message("Library mode")
    # Define the BooksSDK library
    add_library(BooksSDK SHARED ${SOURCES})
else ()
    message("Executable mode")
    # Define the BooksSDK as executable
    add_executable(BooksSDK "main.cpp" ${SOURCES})

    if (NOT BUILD_FOR_ANDROID)
        # Define the external/bin directory
        set(EXTERNAL_BIN_DIR ${CMAKE_SOURCE_DIR}/external/windows/bin)

        # Define the target output directory (build directory)
        set(TARGET_OUTPUT_DIR ${CMAKE_BINARY_DIR})

        # Add a custom post-build command to copy files
        add_custom_command(TARGET BooksSDK POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${EXTERNAL_BIN_DIR} ${TARGET_OUTPUT_DIR}
                COMMENT "Copying external DLLs from ${EXTERNAL_BIN_DIR} to ${TARGET_OUTPUT_DIR}"
        )
    endif ()
endif ()

# Add include directories after the target is defined
if (NOT BUILD_FOR_ANDROID)
    message("Build for windows")

    target_include_directories(BooksSDK PUBLIC
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/external/windows/include)

    # Link external libraries (Zlib, libcurl, etc.)
    target_link_libraries(BooksSDK PUBLIC
            ${CMAKE_SOURCE_DIR}/external/windows/lib/zlib.lib
            ${CMAKE_SOURCE_DIR}/external/windows/lib/libcurl.lib
    )
else ()
    message("Build for android")
    set(CMAKE_SYSTEM_NAME Android)
    set(CMAKE_ANDROID_NDK ${ANDROID_NDK})
    set(CMAKE_ANDROID_ARCH_ABI ${ANDROID_ABI})
    set(CMAKE_ANDROID_PLATFORM ${ANDROID_PLATFORM})

    target_include_directories(BooksSDK PUBLIC
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/external/android/include)

    # Link external libraries (Zlib, libcurl, etc.)
    target_link_libraries(BooksSDK PUBLIC
            log
            ${CMAKE_SOURCE_DIR}/external/android/lib/libz.a
            ${CMAKE_SOURCE_DIR}/external/android/lib/libcurl.a
            ${CMAKE_SOURCE_DIR}/external/android/lib/libssl.a
            ${CMAKE_SOURCE_DIR}/external/android/lib/libcrypto.a
    )
endif ()

# Tests
add_subdirectory(tests)
