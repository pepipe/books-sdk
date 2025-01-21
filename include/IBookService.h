#pragma once
#include <Book.h>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class IBookService
{
public:
    using FetchBooksCallback = std::function<void(std::vector<Book>, std::string)>;
    using FetchBooksJSONCallback = std::function<void(std::string, std::string)>;

    virtual ~IBookService() = default;

    virtual void FetchBooks(const std::string &query, int startIndex, int maxResults, FetchBooksCallback callback) = 0;
    virtual void FetchBooks(const std::string &query, int startIndex, int maxResults, FetchBooksJSONCallback callback) = 0;
    [[nodiscard]] virtual bool IsFavorite(const std::string &bookId) const = 0;
    [[nodiscard]] virtual std::vector<std::string> GetFavoriteBooks() const = 0;
    [[nodiscard]] virtual std::vector<Book> GetFavoriteBooksObjects() const = 0;
    virtual void AddToFavorites(std::string bookId, std::string bookJson) = 0;
    virtual void AddToFavoritesBook(Book &book) = 0;

    /**
      * Encodes a URL string for safe transmission over HTTP.
      * This utility method is needed by implementations of IBookService that deal with URL requests.
      */
    static std::string UrlEncode(const std::string &url)
    {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (const char c: url)
        {
            // Encode only non-alphanumeric characters or reserved URL characters
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                escaped << c;
            } else
            {
                escaped << '%' << std::setw(2) << std::uppercase << static_cast<int>(static_cast<unsigned char>(c));
            }
        }

        return escaped.str();
    }
};
