#pragma once

#include <initializer_list>
#include <print>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <span>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;

    using AuthorContainer = std::set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) {
        std::for_each(books.begin(), books.end(), [this](const auto &book) {
            const auto [author_it, _] = authors_.insert(std::string{book.author});
            books_.emplace_back(book.title, *author_it, book.year, book.genre, book.rating, book.read_count);
        });
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }
    const_iterator begin() const { return books_.begin(); }
    const_iterator end() const { return books_.end(); }
    size_t size() const { return books_.size(); }

    void PushBack(const Book &b) {
        const auto [it, _] = authors_.insert(std::string{b.author});
        books_.emplace_back(b.title, *it, b.year, b.genre, b.rating, b.read_count);
    }

    template <typename... Args>
    void EmplaceBack(Args... args) {
        EmplaceBack_(args...);
    }

    std::span<const Book> GetBooks() const { return books_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

private:
    template <typename... Args>
    void EmplaceBack_(std::string_view title, std::string_view author, Args... args) {
        const auto [it, _] = authors_.insert(std::string{author});
        books_.emplace_back(title, *it, std::forward<Args>(args)...);
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        (size/begin/...)

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
