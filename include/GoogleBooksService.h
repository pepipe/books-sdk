#pragma once

#include "IBookService.h"
#include <../external/include/nlohmann/json.hpp>
#include <vector>
#include <map>

class GoogleBooksService final : public IBookService
{
public:
    void FetchBooks(const std::string &query, int startIndex, int maxResults,
                    std::function<void(std::vector<Book>, std::string)> callback) override;

    bool IsFavorite(const std::string &bookId) const;
    [[nodiscard]] std::vector<Book> GetFavoriteBooks() const override;
    void AddToFavorites(Book& book) override;

private:
    static std::string PerformRequest(const std::string &url);

    std::string _baseUrl = "https://www.googleapis.com/books/v1/volumes";
    std::map<std::string, Book> _favorites;
};
