#include "Book.h"

#include <utility>

Book::Book(std::string id, std::string title, std::string author, std::string description,
           std::string thumbnail, std::string buyLink) :
_id(std::move(id)),
_title(std::move(title)),
_author(std::move(author)),
_description(std::move(description)),
_thumbnail(std::move(thumbnail)),
_buyLink(std::move(buyLink))
{
}

const std::string &Book::GetId() const
{
    return _id;
}

const std::string &Book::GetTitle() const
{
    return _title;
}

const std::string &Book::GetAuthor() const
{
    return _author;
}

const std::string &Book::GetDescription() const
{
    return _description;
}

const std::string &Book::GetThumbnail() const
{
    return _thumbnail;
}

const std::string &Book::GetBuyLink() const
{
    return _buyLink;
}

bool Book::GetIsFavorite() const
{
    return _isFavorite;
}

void Book::SetFavorite(const bool favorite)
{
    _isFavorite = favorite;
}
