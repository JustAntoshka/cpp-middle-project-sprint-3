#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, BookComparator Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, size_t> result;
    std::for_each(cont.begin(), cont.end(), [&result](auto &book) { result[std::string(book.author)]++; });
    return result;
}

template <BookIterator Iter>
auto calculateGenreRatings(Iter begin, Iter end) {
    std::flat_map<Genre, std::pair<double, size_t>> genre_2_rating_sum_map;
    std::for_each(begin, end, [&genre_2_rating_sum_map](auto &&book) {
        auto &[rating, n] = genre_2_rating_sum_map[book.genre];
        rating += book.rating;
        n++;
    });
    std::flat_map<Genre, double> result;
    std::for_each(genre_2_rating_sum_map.begin(), genre_2_rating_sum_map.end(), [&result](auto &&genre_2_rating_sum) {
        auto genre = genre_2_rating_sum.first;
        const auto &rating_sum = genre_2_rating_sum.second;
        result[genre] = rating_sum.first / rating_sum.second;
    });
    return result;
}

template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    double rating =
        std::accumulate(cont.begin(), cont.end(), 0, [](auto acc, auto book) { return acc += book.rating; });
    return rating / cont.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t n) {
    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(n);
    std::for_each_n(cont.begin(), n, [&result](const auto &book) { result.push_back(book); });
    return result;
}

template <BookContainerLike T, BookComparator Comparator = comp::LessByAuthor>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp) {
    std::sort(cont.begin(), cont.end(), comp);
    return sampleRandomBooks(cont, n);
}

}  // namespace bookdb

namespace std {
template <>
struct formatter<std::flat_map<std::string, size_t>> {
    template <typename FormatContext>
    auto format(const std::flat_map<std::string, size_t> &hist, FormatContext &fc) const {
        for (const auto &[author, count] : hist) {
            format_to(fc.out(), "{} - {}\n", author, count);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<std::flat_map<bookdb::Genre, double>> {
    template <typename FormatContext>
    auto format(const std::flat_map<bookdb::Genre, double> &genre_2_rating, FormatContext &fc) const {
        for (const auto &[genre, rating] : genre_2_rating) {
            format_to(fc.out(), "{} - {}\n", genre, rating);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
