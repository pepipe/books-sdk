#include <gtest/gtest.h>
#include "../include/GoogleBooksService.h"

TEST(GoogleBooksServiceTests, UrlEncode_EncodesCorrectly) {
    // Arrange
    const std::string input = "hello world!";
    const std::string expectedOutput = "hello%20world%21";

    // Act
    const std::string result = IBookService::UrlEncode(input);

    // Assert
    EXPECT_EQ(result, expectedOutput);
}