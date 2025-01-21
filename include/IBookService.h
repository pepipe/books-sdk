#pragma once
#include <Book.h>
#include <functional>
#include <string>
#include <vector>

class IBookService
{
public:
    virtual ~IBookService() = default;

    virtual void FetchBooks(const std::string &query, int startIndex, int maxResults,
                            std::function<void(std::vector<Book>, std::string)> callback) = 0;
    [[nodiscard]] virtual std::vector<Book> GetFavoriteBooks() const = 0;
    virtual void AddToFavorites(Book& book) = 0;
};
