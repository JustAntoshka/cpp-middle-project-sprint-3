#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(const Book &lhs, const Book &rhs) const noexcept { return lhs.title < rhs.title; }
    bool operator()(const Book &b, std::string_view title) const noexcept { return b.title < title; }
    bool operator()(std::string_view title, const Book &b) const noexcept { return b.title > title; }
};

struct TransparentStringEqual {
    using is_transparent = void;
    bool operator()(const Book &b, std::string_view title) const noexcept { return b.title == title; }
};

struct TransparentStringHash {
    using is_transparent = void;
    bool operator()(std::string_view title) const noexcept { return std::hash<std::string_view>{}(title); }
};

}  // namespace bookdb
