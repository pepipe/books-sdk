#include <gtest/gtest.h>
#include "../include/GoogleBooksService.h"

class GoogleBooksServiceTests : public ::testing::Test {
protected:
    GoogleBooksService service;
};

TEST_F(GoogleBooksServiceTests, UrlEncode_EncodesCorrectly) {
    // Arrange
    const std::string input = "hello world!";
    const std::string expectedOutput = "hello%20world%21";

    // Act
    const std::string result = IBookService::UrlEncode(input);

    // Assert
    EXPECT_EQ(result, expectedOutput);
}

TEST_F(GoogleBooksServiceTests, AddToFavoritesAddsABookToFavorites)
{
    // Arrange
    std::string bookId = "book1";
    std::string bookJson = "{\"title\": \"Book 1\"}";

    // Act
    service.AddToFavorites(bookId, bookJson);

    // Assert
    EXPECT_TRUE(service.IsFavorite(bookId));
}

TEST_F(GoogleBooksServiceTests, GetFavoriteBooksRetrievesFavoriteBooksCorrectly)
{
    // Arrange
    std::string bookId1 = "book1";
    std::string bookJson1 = "{\"title\": \"Book 1\"}";
    std::string bookId2 = "book2";
    std::string bookJson2 = "{\"title\": \"Book 2\"}";

    // Act
    service.AddToFavorites(bookId1, bookJson1);
    service.AddToFavorites(bookId2, bookJson2);
    std::vector<std::string> favorites = service.GetFavoriteBooks();

    // Assert
    EXPECT_EQ(favorites.size(), 2);
    EXPECT_NE(std::find(favorites.begin(), favorites.end(), bookJson1), favorites.end());
    EXPECT_NE(std::find(favorites.begin(), favorites.end(), bookJson2), favorites.end());
}