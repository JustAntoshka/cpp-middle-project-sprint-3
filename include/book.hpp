#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

constexpr Genre GenreFromString(std::string_view s) {
    static constexpr auto GENRES = std::to_array<std::pair<std::string_view, Genre>>({
        {"Fiction", Genre::Fiction},
        {"NonFiction", Genre::NonFiction},
        {"SciFi", Genre::SciFi},
        {"Biography", Genre::Biography},
        {"Mystery", Genre::Mystery}
    });

    auto result = std::find_if(GENRES.begin(), GENRES.end(), [s](auto&& genre_str_pair){
        return genre_str_pair.first == s;
    });

    return result != GENRES.end() ? result->second : Genre::Unknown;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(std::string_view _title, std::string_view _author, int _year, Genre _genre, double _rating,
                   int _read_count)
        : author(_author), title(_title), year(_year), genre(_genre), rating(_rating), read_count(_read_count) {}

    constexpr Book(std::string_view _title, std::string_view _author, int _year, std::string_view _genre,
                   double _rating, int _read_count)
        : author(_author), title(_title), year(_year), genre(GenreFromString(_genre)), rating(_rating),
          read_count(_read_count) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        return format_to(fc.out(), "{}, \"{}\", {}, {}, Rating: {}, Read count: {}", b.author, b.title, b.year, b.genre,
                         b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
