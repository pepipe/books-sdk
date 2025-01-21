#pragma once
#include <string>

class Book
{
public:
    Book(std::string id, std::string title, std::string author, std::string description, std::string thumbnail,
         std::string buyLink);

    [[nodiscard]] const std::string &GetId() const;
    [[nodiscard]] const std::string &GetTitle() const;
    [[nodiscard]] const std::string &GetAuthor() const;
    [[nodiscard]] const std::string &GetDescription() const;
    [[nodiscard]] const std::string &GetThumbnail() const;
    [[nodiscard]] const std::string &GetBuyLink() const;
    [[nodiscard]] bool GetIsFavorite() const;
    void SetFavorite(bool favorite);

private:
    std::string _id;
    std::string _title;
    std::string _author;
    std::string _description;
    std::string _thumbnail;
    std::string _buyLink;
    bool _isFavorite = false;
};
