#include <iostream>

#include "include/GoogleBooksService.h"

void HandleBooks(const std::string &books, const int statusCode, const std::string &status)
{
    if (statusCode == 200)
    {
        // Check if the operation succeeded
        std::cout << "Fetched books:\n";
        std::cout << books << "\n";
    }
    else
    {
        // Handle an error
        std::cerr << "Error: " << status << "\n";
    }
}


int main()
{
    GoogleBooksService service;
    service.FetchBooks("iOS", 0, 5, HandleBooks);
    bool b = service.IsFavorite("book1");
    return 0;
}
