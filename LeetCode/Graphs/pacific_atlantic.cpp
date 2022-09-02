/**
 * \file
 * \brief
 * \details There is an m x n rectangular island that borders both the
 * Pacific Ocean and Atlantic Ocean. The Pacific Ocean touches the
 * island's left and top edges, and the Atlantic Ocean touches the
 * island's right and bottom edges.
 * \author Vladimir Petrigo
 */
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
#include <vector>

void internal_pa_solver(const std::vector<std::vector<int>> &heights,
                        std::size_t i, std::size_t j, int prev_height,
                        const std::bitset<2> &val,
                        std::vector<std::vector<std::bitset<2>>> &visited)
{
    const auto height = heights.size();
    const auto width = heights.front().size();

    if (i == std::numeric_limits<std::size_t>::max() ||
        j == std::numeric_limits<std::size_t>::max() || i == height ||
        j == width || heights[i][j] < prev_height ||
        (visited[i][j] & val) == val) {
        return;
    }

    visited[i][j] |= val;
    internal_pa_solver(heights, i - 1, j, heights[i][j], visited[i][j],
                       visited);
    internal_pa_solver(heights, i + 1, j, heights[i][j], visited[i][j],
                       visited);
    internal_pa_solver(heights, i, j - 1, heights[i][j], visited[i][j],
                       visited);
    internal_pa_solver(heights, i, j + 1, heights[i][j], visited[i][j],
                       visited);
}

std::vector<std::vector<int>>
pacific_atlantic_solver(const std::vector<std::vector<int>> &heights)
{
    const std::size_t height = heights.size();
    const std::size_t width = heights.front().size();
    std::vector<std::vector<int>> result;
    std::vector<std::vector<std::bitset<2>>> visited(
        height, std::vector<std::bitset<2>>(width, std::bitset<2>{0}));

    for (std::size_t row = 0; row < height; ++row) {
        internal_pa_solver(heights, row, 0, std::numeric_limits<int>::min(),
                           std::bitset<2>{}.set(0), visited);
        internal_pa_solver(heights, row, width - 1,
                           std::numeric_limits<int>::min(),
                           std::bitset<2>{}.set(1), visited);
    }

    for (std::size_t col = 0; col < width; ++col) {
        internal_pa_solver(heights, 0, col, std::numeric_limits<int>::min(),
                           std::bitset<2>{}.set(0), visited);
        internal_pa_solver(heights, height - 1, col,
                           std::numeric_limits<int>::min(),
                           std::bitset<2>{}.set(1), visited);
    }

    for (std::size_t row = 0; row < height; ++row) {
        for (std::size_t col = 0; col < width; ++col) {
            if (visited[row][col].all()) {
                result.push_back(
                    {static_cast<int>(row), static_cast<int>(col)});
            }
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

    std::ios::sync_with_stdio(false);

    // const std::vector<std::vector<int>> heights{
    //     {1, 2, 2, 3, 5}, {3, 2, 3, 4, 4}, {2, 4, 5, 3, 1},
    //     {6, 7, 1, 4, 5}, {5, 1, 1, 2, 4},
    // };
    // const std::vector<std::vector<int>> heights{{1}};
    // const std::vector<std::vector<int>> heights(200, std::vector<int>(200,
    // 100000));
    const std::vector<std::vector<int>> heights{
        {1, 2, 3},
        {8, 9, 4},
        {7, 6, 5},
    };
    const auto result = pacific_atlantic_solver(heights);

    std::cout << result.size() << '\n';

    for (const auto &r : result) {
        std::cout << r[0] << ' ' << r[1] << '\n';
    }

    return 0;
}

TEST_CASE("Pacific-Atlantic basic test")
{
    const std::vector<std::vector<int>> heights{
        {1, 2, 2, 3, 5}, {3, 2, 3, 4, 4}, {2, 4, 5, 3, 1},
        {6, 7, 1, 4, 5}, {5, 1, 1, 2, 4},
    };
    const auto result = pacific_atlantic_solver(heights);
    const auto expected{std::vector<std::vector<int>>{
        {0, 4}, {1, 3}, {1, 4}, {2, 2}, {3, 0}, {3, 1}, {4, 0}}};

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}

TEST_CASE("Pacific-Atlantic single cell")
{
    const std::vector<std::vector<int>> heights{{1}};
    const auto result = pacific_atlantic_solver(heights);
    const auto expected{std::vector<std::vector<int>>{{0, 0}}};

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}

TEST_CASE("Pacific-Atlantic mountains")
{
    const std::vector<std::vector<int>> heights{
        {3, 3, 3, 3, 3, 3}, {3, 0, 3, 3, 0, 3}, {3, 3, 3, 3, 3, 3}};
    const auto result = pacific_atlantic_solver(heights);
    auto expected{std::vector<std::vector<int>>{
        {0, 0},
        {0, 1},
        {0, 2},
        {0, 3},
        {0, 4},
        {0, 5},
        {1, 0},
        {1, 2},
        {1, 3},
        {1, 5},
        {2, 0},
        {2, 1},
        {2, 2},
        {2, 3},
        {2, 4},
        {2, 5},
    }};

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}

TEST_CASE("Pacific-Atlantic max size")
{
    const std::vector<std::vector<int>> heights(200,
                                                std::vector<int>(200, 100000));
    const auto result = pacific_atlantic_solver(heights);
    auto expected{std::vector<std::vector<int>>(200 * 200)};

    for (std::size_t i = 0; i < 200; ++i) {
        for (std::size_t j = 0; j < 200; ++j) {
            expected[i * heights.front().size() + j] =
                std::vector<int>{static_cast<int>(i), static_cast<int>(j)};
        }
    }

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}

TEST_CASE("Pacific-Atlantic spiral case")
{
    const std::vector<std::vector<int>> heights{
        {1, 2, 3},
        {8, 9, 4},
        {7, 6, 5},
    };
    const auto result = pacific_atlantic_solver(heights);
    const auto expected{std::vector<std::vector<int>>{
        {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}}};

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(expected)));
}
