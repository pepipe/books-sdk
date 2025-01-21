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
    virtual ~IBookService() = default;

    virtual void FetchBooks(const std::string &query, int startIndex, int maxResults,
                            std::function<void(std::vector<Book>, std::string)> callback) = 0;
    [[nodiscard]] virtual std::vector<Book> GetFavoriteBooks() const = 0;
    virtual void AddToFavorites(Book& book) = 0;
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
