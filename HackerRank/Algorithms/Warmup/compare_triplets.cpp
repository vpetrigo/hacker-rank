/**
 * \file
 * \brief
 * \details Alice and Bob each created one problem for HackerRank. A
 * reviewer rates the two challenges, awarding points on a scale from 1
 * to 100 for three categories: problem clarity, originality, and
 * difficulty.
 * \author Vladimir Petrigo
 */

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <array>
#include <iostream>

constexpr unsigned INPUT_SIZE{3};

std::pair<unsigned, unsigned>
get_review_results(const std::array<int, INPUT_SIZE> &alice_points,
                   const std::array<int, INPUT_SIZE> &bob_points)
{
    unsigned alice_result{0};
    unsigned bob_result{0};

    for (std::size_t i = 0; i < alice_points.size(); ++i) {
        if (alice_points[i] > bob_points[i]) {
            ++alice_result;
        }
        else if (alice_points[i] < bob_points[i]) {
            ++bob_result;
        }
    }

    return std::make_pair(alice_result, bob_result);
}

int main(int argc, char *argv[])
{
    doctest::Context ctx;

    ctx.applyCommandLine(argc, argv);
    int res = ctx.run();

    if (ctx.shouldExit()) {
        return res;
    }

    std::array<int, INPUT_SIZE> alice_points{};
    std::array<int, INPUT_SIZE> bob_points{};

    std::copy_n(std::istream_iterator<int>(std::cin), INPUT_SIZE,
                std::begin(alice_points));
    std::copy_n(std::istream_iterator<int>(std::cin), INPUT_SIZE,
                std::begin(bob_points));
    const auto result = get_review_results(alice_points, bob_points);

    std::cout << result.first << ' ' << result.second << '\n';

    return res;
}

TEST_CASE("Simple Compare Triplets problem checks")
{
    CHECK(get_review_results({1, 2, 3}, {3, 2, 1}) == std::make_pair(1U, 1U));
    CHECK(get_review_results({5, 6, 7}, {3, 6, 10}) == std::make_pair(1U, 1U));
    CHECK(get_review_results({17, 28, 30}, {99, 16, 8}) ==
          std::make_pair(2U, 1U));
}
