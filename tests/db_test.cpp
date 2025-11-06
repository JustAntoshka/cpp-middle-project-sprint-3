#include <gtest/gtest.h>

#include "book_database.hpp"
#include "statsistics.hpp"

using namespace bookdb;

void CHECK_BOOK(const Book& lhs, const Book& rhs) {
    EXPECT_EQ(lhs.author, rhs.author);
    EXPECT_EQ(lhs.title, rhs.title);
    EXPECT_EQ(lhs.year, rhs.year);
    EXPECT_EQ(lhs.genre, rhs.genre);
    EXPECT_EQ(lhs.read_count, rhs.read_count);
    EXPECT_NEAR(lhs.rating, rhs.rating, 1e-6);
};

void CHECK_BOOKS(std::span<const Book> lhs, const std::vector<Book>& rhs) {
    EXPECT_EQ(lhs.size(), rhs.size());
    for(size_t i = 0; i < lhs.size(); ++i) {
        CHECK_BOOK(lhs[i], rhs[i]);
    }
};

template <typename AuthorCont>
void CHECK_AUTHORS(const AuthorCont& lhs, const AuthorCont& rhs) {
    EXPECT_EQ(lhs, rhs);
};

const Book book1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
const Book book2{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};
const Book book3{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120};

TEST(TestComponentName, SimpleCheck) { EXPECT_EQ(1 + 1, 2); }

TEST(TestBook, CheckGenreString) {
    const auto test = [](std::string_view genre_str, Genre genre){
        Book b{"Title", "Author", 0, genre_str, 0.0, 0};
        EXPECT_EQ(b.genre, genre);
    };

    test("Fiction", Genre::Fiction);
    test("Mystery", Genre::Mystery);
    test("NonFiction", Genre::NonFiction);
    test("SciFi", Genre::SciFi);
    test("Biography", Genre::Biography);
    test("HelloWorld", Genre::Unknown);
}

TEST(TestBookDB, CheckCtor) {
    auto test = [](const auto& act_book_db, const std::vector<Book>& exp_books, const auto& exp_authors, size_t exp_size) {
        CHECK_BOOKS(act_book_db.GetBooks(), exp_books);
        CHECK_AUTHORS(act_book_db.GetAuthors(), exp_authors);
        EXPECT_EQ(act_book_db.size(), exp_size);
    };

    {
        BookDatabase book_db_vec{};
        test(book_db_vec, {}, decltype(book_db_vec)::AuthorContainer{}, 0);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        test(book_db, {book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 1);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
            {book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count}
        }};
        test(book_db, {book1, book2}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
            {book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count},
            {book3.title, book3.author, book3.year, book3.genre, book3.rating, book3.read_count}
        }};
        test(book_db, {book1, book2, book3}, decltype(book_db)::AuthorContainer{"George Orwell", "F. Scott Fitzgerald"}, 3);
    }
}

TEST(TestBookDB, CheckClear) {
    auto test = [](const auto& act_book_db) {
        CHECK_BOOKS(act_book_db.GetBooks(), {});
        CHECK_AUTHORS(act_book_db.GetAuthors(), {});
        EXPECT_EQ(act_book_db.begin(), act_book_db.end());
        EXPECT_EQ(act_book_db.size(), 0);
    };

    {
        BookDatabase book_db{};
        book_db.Clear();
        test(book_db);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.Clear();
        test(book_db);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
            {book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count}
        }};
        book_db.Clear();
        test(book_db);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
            {book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count},
            {book3.title, book3.author, book3.year, book3.genre, book3.rating, book3.read_count}
        }};
        book_db.Clear();
        test(book_db);
    }
}

TEST(TestBookDB, CheckPushBack) {
    auto test = [](const auto& act_book_db, const std::vector<Book>& exp_books, const auto& exp_authors, size_t exp_size) {
        CHECK_BOOKS(act_book_db.GetBooks(), exp_books);
        CHECK_AUTHORS(act_book_db.GetAuthors(), exp_authors);
        EXPECT_EQ(act_book_db.size(), exp_size);
    };

    {
        BookDatabase book_db{};
        book_db.PushBack(book1);
        test(book_db, {book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 1);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.PushBack(book1);
        test(book_db, {book1, book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.PushBack(book2);
        test(book_db, {book1, book2}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.PushBack(book3);
        test(book_db, {book1, book3}, decltype(book_db)::AuthorContainer{"George Orwell", "F. Scott Fitzgerald"}, 2);
    }
}

TEST(TestBookDB, CheckEmplaceBack) {
    auto test = [](const auto& act_book_db, const std::vector<Book>& exp_books, const auto& exp_authors, size_t exp_size) {
        CHECK_BOOKS(act_book_db.GetBooks(), exp_books);
        CHECK_AUTHORS(act_book_db.GetAuthors(), exp_authors);
        EXPECT_EQ(act_book_db.size(), exp_size);
    };

    {
        BookDatabase book_db{};
        book_db.EmplaceBack(book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count);
        test(book_db, {book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 1);
    }
    {
        BookDatabase book_db{};
        Book book1_cp = book1;
        book_db.EmplaceBack(std::move(book1_cp.title), std::move(book1_cp.author), std::move(book1_cp.year), std::move(book1_cp.genre), std::move(book1_cp.rating), std::move(book1_cp.read_count));
        test(book_db, {book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 1);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.EmplaceBack(book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count);
        test(book_db, {book1, book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        Book book1_cp = book1;
        book_db.EmplaceBack(std::move(book1_cp.title), std::move(book1_cp.author), std::move(book1_cp.year), std::move(book1_cp.genre), std::move(book1_cp.rating), std::move(book1_cp.read_count));
        test(book_db, {book1, book1}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.EmplaceBack(book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count);
        test(book_db, {book1, book2}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        Book book2_cp = book2;
        book_db.EmplaceBack(std::move(book2_cp.title), std::move(book2_cp.author), std::move(book2_cp.year), std::move(book2_cp.genre), std::move(book2_cp.rating), std::move(book2_cp.read_count));
        test(book_db, {book1, book2}, decltype(book_db)::AuthorContainer{"George Orwell"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        book_db.EmplaceBack(book3.title, book3.author, book3.year, book3.genre, book3.rating, book3.read_count);
        test(book_db, {book1, book3}, decltype(book_db)::AuthorContainer{"George Orwell", "F. Scott Fitzgerald"}, 2);
    }
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
        }};
        Book book3_cp = book3;
        book_db.EmplaceBack(std::move(book3_cp.title), std::move(book3_cp.author), std::move(book3_cp.year), std::move(book3_cp.genre), std::move(book3_cp.rating), std::move(book3_cp.read_count));
        test(book_db, {book1, book3}, decltype(book_db)::AuthorContainer{"George Orwell", "F. Scott Fitzgerald"}, 2);
    }
}

TEST(TestStatistics, CheckBuildAuthorHistogramFlat) {
    {
        BookDatabase book_db{};
        const auto hist = buildAuthorHistogramFlat(book_db);
        const std::flat_map<std::string, size_t> exp_hist{};
        EXPECT_EQ(hist, exp_hist);
    }
    {
        const std::flat_map<std::string, size_t> exp_hist{ {"George Orwell", 1} };

        BookDatabase book_db_1{{ book1 }};
        const auto hist_1 = buildAuthorHistogramFlat(book_db_1);
        EXPECT_EQ(hist_1, exp_hist);

        BookDatabase book_db_2{};
        book_db_2.PushBack(book1);
        const auto hist_2 = buildAuthorHistogramFlat(book_db_2);
        EXPECT_EQ(hist_2, exp_hist);

        BookDatabase book_db_3{};
        book_db_3.EmplaceBack(book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count);
        const auto hist_3 = buildAuthorHistogramFlat(book_db_3);
        EXPECT_EQ(hist_3, exp_hist);
    }
    {
        const std::flat_map<std::string, size_t> exp_hist{
            {"George Orwell", 2},
            {"F. Scott Fitzgerald", 1}
        };

        BookDatabase book_db_1{{
            {book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count},
            {book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count},
            {book3.title, book3.author, book3.year, book3.genre, book3.rating, book3.read_count}
        }};
        const auto hist = buildAuthorHistogramFlat(book_db_1);
        EXPECT_EQ(hist, exp_hist);

        BookDatabase book_db_2{};
        book_db_2.PushBack(book1);
        book_db_2.PushBack(book2);
        book_db_2.PushBack(book3);
        const auto hist_2 = buildAuthorHistogramFlat(book_db_2);
        EXPECT_EQ(hist_2, exp_hist);

        BookDatabase book_db_3{};
        book_db_3.EmplaceBack(book1.title, book1.author, book1.year, book1.genre, book1.rating, book1.read_count);
        book_db_3.EmplaceBack(book2.title, book2.author, book2.year, book2.genre, book2.rating, book2.read_count);
        book_db_3.EmplaceBack(book3.title, book3.author, book3.year, book3.genre, book3.rating, book3.read_count);
        const auto hist_3 = buildAuthorHistogramFlat(book_db_3);
        EXPECT_EQ(hist_2, exp_hist);
    }
}

TEST(TestStatistics, CheckCalculateGenreRatings) {
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, Genre::SciFi, 1.0, book1.read_count},
            {book2.title, book2.author, book2.year, Genre::NonFiction, 2.0, book2.read_count},
            {book3.title, book3.author, book3.year, Genre::Fiction, 3.0, book3.read_count}
        }};
        
        auto ratings = calculateGenreRatings(book_db.begin(), book_db.end());
        std::flat_map<Genre, double> exp_ratings{
            {Genre::SciFi, 1.0},
            {Genre::NonFiction, 2.0},
            {Genre::Fiction, 3.0}
        }; 
        EXPECT_EQ(ratings, exp_ratings);
    }
}

TEST(TestStatistics, CheckAverageRating) {
    {
        BookDatabase book_db{{
            {book1.title, book1.author, book1.year, Genre::SciFi, 1.0, book1.read_count},
            {book2.title, book2.author, book2.year, Genre::NonFiction, 2.0, book2.read_count},
            {book3.title, book3.author, book3.year, Genre::Fiction, 3.0, book3.read_count}
        }};
        
        auto avg_rating = calculateAverageRating(book_db);
        EXPECT_EQ(avg_rating, 2.0);
    }
}

TEST(TestStatistics, CheckSampleRandomBooks) {
    {
        BookDatabase book_db{book1, book2, book3};

        auto rnd_books = sampleRandomBooks(book_db, 1);
        EXPECT_EQ(rnd_books.size(), 1);
        CHECK_BOOK(rnd_books[0], book1);
    }
}

TEST(TestStatistics, CheckGetTopNBy) {
    {
        BookDatabase book_db{};
        book_db.EmplaceBack("Book A 1", "Author A", 42, Genre::Mystery, 25.0, 100);
        book_db.EmplaceBack("Book A 2", "Author A", 43, Genre::Mystery, 20.0, 110);
        book_db.EmplaceBack("Book B 1", "Author B", 44, Genre::Mystery, 15.0, 120);

        auto top_books = getTopNBy(book_db, 1, comp::LessByRating{});
        EXPECT_EQ(top_books.size(), 1);
        CHECK_BOOK(top_books[0], {"Book B 1", "Author B", 44, Genre::Mystery, 15.0, 120});
    }
}

TEST(TestFilterBooks, CheckFilter) {
    {
        BookDatabase book_db{};
        book_db.EmplaceBack("Book A 1", "Author A", 42, Genre::Mystery, 25.0, 100);
        book_db.EmplaceBack("Book A 2", "Author A", 43, Genre::Mystery, 20.0, 110);
        book_db.EmplaceBack("Book B 1", "Author B", 44, Genre::Mystery, 15.0, 120);
        
        auto filtered_books = filterBooks(book_db.begin(), book_db.end(), [](auto&& book){
            return book.author == "Author A";
        });

        EXPECT_EQ(filtered_books.size(), 2);
        CHECK_BOOK(filtered_books.at(0), {"Book A 1", "Author A", 42, Genre::Mystery, 25.0, 100});
        CHECK_BOOK(filtered_books.at(1), {"Book A 2", "Author A", 43, Genre::Mystery, 20.0, 110});
    }
}
