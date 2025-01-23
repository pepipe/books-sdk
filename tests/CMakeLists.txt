# Tests
# Fetch GoogleTest from the official repository
include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(gtest)

file(GLOB TEST_SOURCES "*.cpp")
add_executable(BooksSDKTests ${TEST_SOURCES})

target_link_libraries(BooksSDKTests PRIVATE gtest gtest_main BooksSDK)

# Enable CTest for testing
add_test(NAME BooksSDKTests COMMAND BooksSDKTests)