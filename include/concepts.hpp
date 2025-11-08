#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T cont) {
    cont.begin();
    cont.end();
    requires std::same_as<std::remove_cvref_t<decltype(*cont.begin())>, Book>;
};

template <typename T>
concept BookIterator = requires(T iter) { 
    { *iter } -> std::same_as<Book&>;
};

template <typename S, typename I>
concept BookSentinel = requires(I i, S s) {
    { i != s } -> std::convertible_to<bool>;
    { i == s } -> std::convertible_to<bool>;
};

template <typename P>
concept BookPredicate = requires(P p, const Book& b) {
    { p(b) } -> std::same_as<bool>;
};

template <typename C>
concept BookComparator = requires(C comp, const Book& lhs, const Book& rhs) {
    { comp(lhs, rhs) } -> std::same_as<bool>;
};

}  // namespace bookdb