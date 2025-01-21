#include <Book.h>
#include <GoogleBooksService.h>
#include <iostream>
#include <vector>

void HandleBooks(const std::vector<Book> &books, const std::string &status)
{
    if (status == "OK")
    {
        // Check if the operation succeeded
        std::cout << "Fetched books:\n";
        for (const auto &book: books)
        {
            std::cout << "Title: " << book.GetTitle() << ", Author: " << book.GetAuthor() << "\n";
        }
    } else
    {
        // Handle an error
        std::cerr << "Error: " << status << "\n";
    }
}


int main()
{
    std::vector<Book> books;
    GoogleBooksService service;
    service.FetchBooks("Harry Potter", 0, 20, HandleBooks);
}
