#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

    auto YearBetween(int from, int to) {
        return [from, to](const Book& book) {
            return from < book.year && book.year < to;
        };
    }

    auto RatingAbove(auto min_rating) {
        return [min_rating](const Book& book){
            return min_rating < book.rating;
        };
    }

    auto GenreIs(auto genre) {
        return [genre](const Book& book){
            return book.genre == genre;
        };
    }

    template <BookPredicate... Preds>
    auto all_of(Preds... preds) {
        return [preds...](const Book& b){
            return (... && preds(b));
        };
    }

    template <BookPredicate... Preds>
    auto any_of(Preds... preds) {
        return [preds...](const Book& b){
            return (... || preds(b));
        };
    }

    template <BookIterator Iter, BookPredicate Predicate>
    auto filterBooks(Iter begin, Iter end, Predicate pred) {
        std::vector<std::reference_wrapper<const Book>> result;
        result.reserve(std::distance(begin, end));
        std::copy_if(begin, end, std::back_inserter(result), pred);
        return result;
    }

}  // namespace bookdb