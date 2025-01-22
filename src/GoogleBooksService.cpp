#include "GoogleBooksService.h"

#include <iostream>
#include <../external/include/curl/curl.h>

// Helper function to write cURL response to a string
size_t WriteCallback(void *contents, const size_t size, const size_t nmemb, std::string *s)
{
    const size_t totalSize = size * nmemb;
    s->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

void GoogleBooksService::FetchBooks(const std::string &query, const int startIndex, const int maxResults,
    const FetchBooksJSONCallback callback)
{
    const std::string encodedQuery = IBookService::UrlEncode(query);
    const std::string url = _baseUrl + "?q=" + encodedQuery + "&maxResults=" + std::to_string(maxResults) +
                            "&startIndex=" + std::to_string(startIndex);

    try
    {
        const std::string response = PerformRequest(url);
        callback(response, 200, "OK");
    } catch (const std::exception &ex)
    {
        std::cerr << "Error performing request: " << ex.what() << std::endl;

        callback({}, 500, "Internal Server Error");
    }
}

void GoogleBooksService::AddToFavorites(const std::string& bookId, std::string bookJson)
{
    _favorites.emplace(bookId, std::move(bookJson));
}

bool GoogleBooksService::IsFavorite(const std::string& bookId) const
{
    return _favorites.find(bookId) != _favorites.end();
}

std::vector<std::string> GoogleBooksService::GetFavoriteBooks() const
{
    std::vector<std::string> favoriteBooks;
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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
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
