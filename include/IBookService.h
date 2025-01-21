#pragma once
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class IBookService
{
public:
    using FetchBooksJSONCallback = std::function<void(std::string, int statusCode, std::string)>;

    virtual ~IBookService() = default;

    virtual void FetchBooks(const std::string &query, int startIndex, int maxResults, FetchBooksJSONCallback callback) = 0;
    [[nodiscard]] virtual bool IsFavorite(const std::string &bookId) const = 0;
    [[nodiscard]] virtual std::vector<std::string> GetFavoriteBooks() const = 0;
    virtual void AddToFavorites(const std::string& bookId, std::string bookJson) = 0;

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
