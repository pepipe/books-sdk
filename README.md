# BOOKS SDK
## Test description - Book Store
Book Store consists on showing a simple 2-column list of available books about mobile development.
Using google’s api for books, the app should fetch and display the Thumbnail of a few books at a time and load more as the user scroll’s through the list.

- [Rest API](https://developers.google.com/books/docs/v1/getting_started#REST)
- [Example API Call](https://www.googleapis.com/books/v1/volumes?q=ios&maxResults=20&startIndex=0)

The list should also have a button to filter/show only books that the user has set as favorite.

When the user clicks on one of the books, the app should present a detailed view displaying the most relevant information of the book: Title, Author, Description and, if available, a Buy link.

In the detail view, the user can also favorite or “unfavorite” a book. This option should be stored locally so it persists through each app usage.

Clicking on the Buy link should open the link on safari/chrome.

This app must be build fallowing this rules:
1. All the API integrations must be built using C++.
    1. The app should call this api method and receive a callback with the API result (a json object or
an error).
2. An APP built using Objective-c/Java that import and use those C++ class.

It is very important show your experience with testing;

Nice to have: Favorites in local storage using C++

## Project Setup - Windows
Using C++ 17 standard with CMake. Third party libraries were used: libcurl. 
I used CLion as IDE with MinGW compiler
1. Need to install CMake.
2. Install Oracle OpenJDK23
3. Then need to install JavaFX: https://gluonhq.com/products/javafx/
4. Create a JAVA_HOME and JAVAFX_HOME env variables that point to the sdks.
5. Add to PATH %JAVA_HOME%\bin and %JAVAFX_HOME%\lib

This should be enough to build BooksSDK dll. 
Inside <project root>/cmake-build-<debug|release> there's a dll file.
Rename it to from libBooksSDK.dll to BooksSDK.dll. 