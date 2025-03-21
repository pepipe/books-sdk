# Tests
# Fetch GoogleTest from the official repository
include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(gtest)

add_definitions(-DTESTING)

file(GLOB TEST_SOURCES "*.cpp")
set(APP_SOURCES
        ${CMAKE_SOURCE_DIR}/src/GoogleBooksService.cpp
)
add_executable(BooksSDKTests ${TEST_SOURCES} ${APP_SOURCES})
target_include_directories(BooksSDKTests PUBLIC ${CMAKE_SOURCE_DIR}/include)
if (NOT BUILD_FOR_ANDROID)
    target_link_libraries(BooksSDKTests PRIVATE
            ${CMAKE_SOURCE_DIR}/external/windows/lib/zlib.lib
            ${CMAKE_SOURCE_DIR}/external/windows/lib/libcurl.lib
    )
else ()
    target_link_libraries(BooksSDKTests PRIVATE
            ${CMAKE_SOURCE_DIR}/external/android/lib/libz.a
            ${CMAKE_SOURCE_DIR}/external/android/lib/libcurl.a
            ${CMAKE_SOURCE_DIR}/external/android/lib/libssl.a
    )
endif ()

if(BUILD_AS_LIB)
target_link_libraries(BooksSDKTests PRIVATE BooksSDK gtest gtest_main)
else ()
    target_link_libraries(BooksSDKTests PRIVATE gtest gtest_main)
endif ()

# Enable CTest for testing
enable_testing()
add_test(NAME BooksSDKTests COMMAND BooksSDKTests)