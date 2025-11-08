#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return lhs.author < rhs.author; }
};

struct GreaterByAuthor {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return LessByAuthor{}(rhs, lhs); }
};

struct LessByTitle {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return lhs.title < rhs.title; }
};

struct GreaterByTitle {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return LessByTitle{}(rhs, lhs); }
};

struct LessByPopularity {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return lhs.read_count > rhs.read_count; }
};

struct GreaterByPopularity {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return LessByPopularity{}(rhs, lhs); }
};

struct LessByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return lhs.rating < rhs.rating; }
};

struct GreaterByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) { return LessByRating{}(rhs, lhs); }
};

}  // namespace bookdb::comp