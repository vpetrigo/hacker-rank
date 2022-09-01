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
#include <vector>

bool internal_pa_solver(const std::vector<std::vector<int>> &heights,
                        std::size_t i, std::size_t j,
                        std::bitset<2> &is_pacific_atlantic,
                        std::vector<std::vector<unsigned char>> &visited)
{
    bool is_pacific = i - 1 > i || j - 1 > j;
    bool is_atlantic =
        i + 1 == heights.size() || j + 1 == heights.front().size();

    if (visited[i][j] != 0) {
        return false;
    }

    if (is_pacific) {
        is_pacific_atlantic.set(0);
    }

    if (is_atlantic) {
        is_pacific_atlantic.set(1);
    }

    if (is_pacific_atlantic.all()) {
        return true;
    }

    visited[i][j] = 1;

    // if (!is_pacific && heights[i][j] >= heights[i - 1][j]) {
    if (i - 1 < i && heights[i][j] >= heights[i - 1][j]) {
        internal_pa_solver(heights, i - 1, j, is_pacific_atlantic, visited);
    }

    // if (!is_atlantic && heights[i][j] >= heights[i + 1][j]) {
    if (i + 1 < heights.size() && heights[i][j] >= heights[i + 1][j]) {
        internal_pa_solver(heights, i + 1, j, is_pacific_atlantic, visited);
    }

    // if (!is_pacific && heights[i][j] >= heights[i][j - 1]) {
    if (j - 1 < j && heights[i][j] >= heights[i][j - 1]) {
        internal_pa_solver(heights, i, j - 1, is_pacific_atlantic, visited);
    }

    // if (!is_atlantic && heights[i][j] >= heights[i][j + 1]) {
    if (j + 1 < heights.front().size() && heights[i][j] >= heights[i][j + 1]) {
        internal_pa_solver(heights, i, j + 1, is_pacific_atlantic, visited);
    }

    return false;
}

std::vector<std::vector<int>>
pacific_atlantic_solver(const std::vector<std::vector<int>> &heights)
{
    const std::size_t height = heights.size();
    const std::size_t width = heights.front().size();
    std::vector<std::vector<int>> result;
    std::vector<std::vector<unsigned char>> visited(
        height, std::vector<unsigned char>(width, 0));

    for (std::size_t row = 0; row < height; ++row) {
        for (std::size_t col = 0; col < width; ++col) {
            std::bitset<2> is_pacific_atlantic;

            internal_pa_solver(heights, row, col, is_pacific_atlantic, visited);
            std::for_each(std::begin(visited), std::end(visited), [](auto &el) {
                std::fill(std::begin(el), std::end(el), 0);
            });

            if (is_pacific_atlantic.all()) {
                result.emplace_back(std::vector<int>{static_cast<int>(row),
                                                     static_cast<int>(col)});
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
                     std::cbegin(result)));
}

TEST_CASE("Pacific-Atlantic single cell")
{
    const std::vector<std::vector<int>> heights{{1}};
    const auto result = pacific_atlantic_solver(heights);
    const auto expected{std::vector<std::vector<int>>{{0, 0}}};

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(result)));
}

TEST_CASE("Pacific-Atlantic max size")
{
    const std::vector<std::vector<int>> heights(200,
                                                std::vector<int>(200, 100000));
    const auto result = pacific_atlantic_solver(heights);
    auto expected{std::vector<std::vector<int>>(200 * 200)};

    for (std::size_t i = 0; i < 200; ++i) {
        for (std::size_t j = 0; j < 200; ++j) {
            expected[i + j] =
                std::vector<int>{static_cast<int>(i), static_cast<int>(j)};
        }
    }

    CHECK(std::size(result) == std::size(expected));
    CHECK(std::equal(std::cbegin(result), std::cend(result),
                     std::cbegin(result)));
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
                     std::cbegin(result)));
}
