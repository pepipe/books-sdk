#pragma once

#include "IBookService.h"
#include <vector>
#include <map>

class GoogleBooksService final : public IBookService
{
public:
    void FetchBooks(const std::string &query, int startIndex, int maxResults, FetchBooksJSONCallback callback) override;

    [[nodiscard]] bool IsFavorite(const std::string &bookId) override;
    [[nodiscard]] std::vector<std::string> GetFavoriteBooks() override;
    void AddToFavorites(const std::string& bookId, std::string bookJson) override;
    void RemoveFromFavorites(const std::string& bookId) override;
    [[nodiscard]] std::string GetFavoritesFilePath() const;
    void SetFavoritesFilePath(const std::string& favoritesFilePath);

private:
    void SaveFavorites() const;
    void LoadFavorites();

    std::string _baseUrl = "https://www.googleapis.com/books/v1/volumes";
    std::string _favoritesFilePath = "favorites.txt";
    std::map<std::string, std::string> _favorites;
    bool _favoritesLoaded = false;
};
