#include "GoogleBooksService.h"
#include <curl/curl.h>

// Helper function to write cURL response to a string
size_t WriteCallback(void *contents, const size_t size, const size_t nmemb, std::string *s)
{
    const size_t totalSize = size * nmemb;
    s->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

void GoogleBooksService::FetchBooks(const std::string &query, const int startIndex, const int maxResults,
                                    const std::function<void(std::vector<Book>, std::string)> callback)
{
    const std::string url = _baseUrl + "?q=" + query + "&maxResults=" + std::to_string(maxResults) +
                      "&startIndex=" + std::to_string(startIndex);

    try
    {
        std::string response = PerformRequest(url);

        auto jsonResponse = nlohmann::json::parse(response);
        std::vector<Book> books;

        for (const auto &item: jsonResponse["items"])
        {
            std::string id = item["id"];
            std::string title = item["volumeInfo"]["title"];
            std::string author = item["volumeInfo"].contains("authors") ? item["volumeInfo"]["authors"][0] : "Unknown";
            std::string description = item["volumeInfo"].contains("description")
                                          ? item["volumeInfo"]["description"]
                                          : "No description available.";
            std::string thumbnail = item["volumeInfo"].contains("imageLinks")
                                        ? item["volumeInfo"]["imageLinks"]["thumbnail"]
                                        : "";
            std::string buyLink = item["saleInfo"].contains("buyLink") ? item["saleInfo"]["buyLink"] : "";

            books.emplace_back(id, title, author, description, thumbnail, buyLink);
        }

        callback(books, "OK");
    } catch (const std::exception &ex)
    {
        callback({}, ex.what());
    }
}

void GoogleBooksService::AddToFavorites(Book& book)
{
    _favorites.emplace(book.GetId(), book);
}

bool GoogleBooksService::IsFavorite(const std::string &bookId) const
{
    return _favorites.find(bookId) != _favorites.end();
}

std::vector<Book> GoogleBooksService::GetFavoriteBooks() const
{
    std::vector<Book> favoriteBooks;
    for (const auto &[id, book]: _favorites)
    {
        favoriteBooks.push_back(book);
    }
    return favoriteBooks;
}

std::string GoogleBooksService::PerformRequest(const std::string &url)
{
    std::string response;

    if (CURL *curl = curl_easy_init())
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        const CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK)
        {
            throw std::runtime_error("Error performing request: " + std::string(curl_easy_strerror(res)));
        }
    }

    return response;
}
