/**
 * \file
 * \brief
 * \author Vladimir Petrigo
 */
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <vector>

constexpr char STONE = '#';
constexpr char OBSTACLE = '*';
constexpr char EMPTY = '.';

std::vector<std::vector<char>> rotateTheBox(std::vector<std::vector<char>> &box)
{
    const std::size_t n = box.size();
    const std::size_t m = box.front().size();
    std::vector<std::vector<char>> result(m, std::vector<char>(n));

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t empty_pos = m - 1;
        for (std::size_t j = m - 1; j < m; --j) {
            if (box[i][j] == OBSTACLE) {
                empty_pos = j - 1;
            }
            else if (box[i][j] == STONE) {
                box[i][j] = EMPTY;
                box[i][empty_pos] = STONE;
                --empty_pos;
            }
        }
    }

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < m; ++j) {
            result[j][i] = box[n - 1 - i][j];
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    doctest::Context ctx;

    ctx.applyCommandLine(argc, argv);

    int res = ctx.run();

    if (ctx.shouldExit()) {
        return res;
    }

    return 0;
}

TEST_CASE("Rotating the box basic test")
{
    std::vector<std::vector<char>> input{{'#', '.', '#'}};
    const std::vector<std::vector<char>> expected{{'.'}, {'#'}, {'#'}};
    const auto result = rotateTheBox(input);

    CHECK_EQ(result.size(), expected.size());
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}

TEST_CASE("Rotating the box two row box")
{
    std::vector<std::vector<char>> input{
        {{'#', '.', '*', '.'}, {'#', '#', '*', '.'}}};
    const std::vector<std::vector<char>> expected{
        {'#', '.'}, {'#', '#'}, {'*', '*'}, {'.', '.'}};
    const auto result = rotateTheBox(input);

    CHECK_EQ(result.size(), expected.size());
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}

TEST_CASE("Rotating the box three row box")
{
    std::vector<std::vector<char>> input{{
        {'#', '#', '*', '.', '*', '.'},
        {'#', '#', '#', '*', '.', '.'},
        {'#', '#', '#', '.', '#', '.'},
    }};
    const std::vector<std::vector<char>> expected{
        {'.', '#', '#'}, {'.', '#', '#'}, {'#', '#', '*'},
        {'#', '*', '.'}, {'#', '.', '*'}, {'#', '.', '.'},
    };
    const auto result = rotateTheBox(input);

    CHECK_EQ(result.size(), expected.size());
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}
