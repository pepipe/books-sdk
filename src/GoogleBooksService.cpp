#include "GoogleBooksService.h"

#include <CurlHttpRequest.h>
#include <fstream>
#include <iostream>

void GoogleBooksService::FetchBooks(const std::string &query, const int startIndex, const int maxResults,
                                    const FetchBooksJSONCallback callback)
{
    const std::string encodedQuery = IBookService::UrlEncode(query);
    const std::string url = _baseUrl + "?q=" + encodedQuery + "&maxResults=" + std::to_string(maxResults) +
                            "&startIndex=" + std::to_string(startIndex);

    try
    {
#ifndef TESTING
        CurlHttpRequest request;
        const std::string response = request.PerformRequest(url);
        callback(response, 200, "OK");
#endif
    } catch (const std::exception &ex)
    {
        std::cerr << "Error performing request: " << ex.what() << std::endl;

        callback({}, 500, "Internal Server Error");
    }
}

bool GoogleBooksService::IsFavorite(const std::string& bookId) const
{
    return _favorites.find(bookId) != _favorites.end();
}

std::vector<std::string> GoogleBooksService::GetFavoriteBooks()
{
    if (!_favoritesLoaded)
    {
        LoadFavorites();
    }
    std::vector<std::string> favoriteBooks;
    for (const auto &[id, book]: _favorites)
    {
        favoriteBooks.push_back(book);
    }
    return favoriteBooks;
}

void GoogleBooksService::AddToFavorites(const std::string& bookId, std::string bookJson)
{
    _favorites.emplace(bookId, std::move(bookJson));
    SaveFavorites();
}

void GoogleBooksService::RemoveFromFavorites(const std::string& bookId)
{
    _favorites.erase(bookId);
    SaveFavorites();
}


void GoogleBooksService::SaveFavorites() const
{
    if (std::ofstream file("favorites.txt"); file.is_open()) {
        for (const auto& kv : _favorites) {
            file << kv.first << "\n" << kv.second << "\n";
        }
        file.close();
    }
}

void GoogleBooksService::LoadFavorites() {
    if (std::ifstream file("favorites.txt"); file.is_open()) {
        std::string key;
        std::string value;
        while (std::getline(file, key)) {
            if (std::getline(file, value)) {
                _favorites[key] = value;
            }
        }
        file.close();
    }
}
