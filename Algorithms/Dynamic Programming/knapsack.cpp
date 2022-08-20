/**
 * \file
 * \brief Just playing around with knapsack 0/1 problem
 * \author Vladimir Petrigo
 */

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <array>
#include <iostream>
#include <unordered_map>
#include <vector>

enum class KnapsackStrategy {
    TOP_DOWN,
    TOP_DOWN_W_MEMO,
    BOTTOM_UP,
};

using KnapsackSolver = int (*)(const std::vector<unsigned> &profit,
                               const std::vector<unsigned> &weight,
                               int capacity);

int profit_calc_top_down(
    const std::vector<unsigned>::const_iterator &profit_start,
    const std::vector<unsigned>::const_iterator &profit_end,
    const std::vector<unsigned>::const_iterator &weight_start,
    const std::vector<unsigned>::const_iterator &weight_end, int capacity,
    int profit = 0)
{
    if (capacity < 0) {
        return 0;
    }

    if (capacity == 0 || profit_start == profit_end) {
        return profit;
    }

    const unsigned item_weight{*weight_start};
    const unsigned item_cost{*profit_start};

    int result = std::max(
        profit_calc_top_down(profit_start + 1, profit_end, weight_start + 1,
                             weight_end,
                             capacity - static_cast<int>(item_weight),
                             profit + static_cast<int>(item_cost)),
        profit_calc_top_down(profit_start + 1, profit_end, weight_start + 1,
                             weight_end, capacity, profit));

    return result;
}

int profit_calc_top_down_with_memo(
    const std::vector<unsigned>::const_iterator &profit_start,
    const std::vector<unsigned>::const_iterator &profit_end,
    const std::vector<unsigned>::const_iterator &weight_start,
    const std::vector<unsigned>::const_iterator &weight_end,
    std::vector<std::vector<int>> &memo, int capacity)
{
    if (capacity <= 0 || profit_start == profit_end) {
        return 0;
    }

    int element{static_cast<int>(std::distance(profit_start, profit_end)) - 1};

    if (memo[element][capacity] != -1) {
        return memo[element][capacity];
    }

    const unsigned item_weight{*weight_start};
    const unsigned item_cost{*profit_start};

    memo[element][capacity] =
        std::max(item_weight <= capacity
                     ? static_cast<int>(item_cost) +
                           profit_calc_top_down_with_memo(
                               profit_start + 1, profit_end, weight_start + 1,
                               weight_end, memo,
                               capacity - static_cast<int>(item_weight))
                     : 0,
                 profit_calc_top_down_with_memo(profit_start + 1, profit_end,
                                                weight_start + 1, weight_end,
                                                memo, capacity));

    return memo[element][capacity];
}

int get_max_profit_top_down(const std::vector<unsigned> &profit,
                            const std::vector<unsigned> &weight, int capacity)
{
    return profit_calc_top_down(std::cbegin(profit), std::cend(profit),
                                std::cbegin(weight), std::cend(weight),
                                capacity);
}

int get_max_profit_top_down_memo(const std::vector<unsigned> &profit,
                                 const std::vector<unsigned> &weight,
                                 int capacity)
{
    std::vector<std::vector<int>> memo(profit.size(),
                                       std::vector<int>(capacity + 1, -1));

    return profit_calc_top_down_with_memo(
        std::cbegin(profit), std::cend(profit), std::cbegin(weight),
        std::cend(weight), memo, capacity);
}

int get_max_profit_bottom_up(const std::vector<unsigned> &profit,
                             const std::vector<unsigned> &weight, int capacity)
{
    std::vector<std::vector<int>> solution(profit.size(),
                                           std::vector<int>(capacity + 1, 0));

    for (size_t avail_capacity = 0; avail_capacity <= capacity;
         ++avail_capacity) {
        if (weight.front() <= avail_capacity) {
            solution[0][avail_capacity] = static_cast<int>(profit.front());
        }
    }

    for (size_t i = 1; i < profit.size(); ++i) {
        for (size_t avail_capacity = 0; avail_capacity <= capacity;
             ++avail_capacity) {
            int profit1{0};
            int profit2{0};

            if (weight[i] <= avail_capacity) {
                profit1 = static_cast<int>(profit[i]) +
                          solution[i - 1][avail_capacity - weight[i]];
            }

            profit2 = solution[i - 1][avail_capacity];
            solution[i][avail_capacity] = std::max(profit1, profit2);
        }
    }

    return solution[profit.size() - 1][capacity];
}

int get_max_profit(const std::vector<unsigned> &profit,
                   const std::vector<unsigned> &weight, int capacity,
                   KnapsackStrategy strategy)
{
    const std::unordered_map<KnapsackStrategy, KnapsackSolver> handlers{
        {KnapsackStrategy::TOP_DOWN, get_max_profit_top_down},
        {KnapsackStrategy::TOP_DOWN_W_MEMO, get_max_profit_top_down_memo},
        {KnapsackStrategy::BOTTOM_UP, get_max_profit_bottom_up},
    };

    return handlers.at(strategy) != nullptr
               ? handlers.at(strategy)(profit, weight, capacity)
               : 0;
}

int main(int argc, char **argv)
{
    doctest::Context ctx;

    ctx.applyCommandLine(argc, argv);

    int res = ctx.run();

    if (ctx.shouldExit()) {
        return res;
    }

    std::ios::sync_with_stdio(false);
    unsigned items;
    unsigned knapsack_size;

    std::cin >> items >> knapsack_size;

    std::vector<unsigned> profit;
    profit.reserve(items);
    std::vector<unsigned> weight;
    weight.reserve(items);

    std::copy_n(std::istream_iterator<unsigned>(std::cin), items,
                std::back_inserter(profit));
    std::copy_n(std::istream_iterator<unsigned>(std::cin), items,
                std::back_inserter(weight));

    int result = get_max_profit(profit, weight, static_cast<int>(knapsack_size),
                                KnapsackStrategy::TOP_DOWN_W_MEMO);
    std::cout << "Total knapsack profit: " << result << '\n';

    return 0;
}

struct TestCases {
    const unsigned knapsack_size;
    const std::vector<unsigned> profit;
    const std::vector<unsigned> weight;
    const int expected_result;
};

TEST_CASE("Knapsack top-down implementation")
{
    const std::vector<TestCases> cases{
        {5, {4, 5, 3, 7}, {2, 3, 1, 4}, 10},
        {7, {1, 6, 10, 16}, {1, 2, 3, 5}, 22},
        {6, {1, 6, 10, 16}, {1, 2, 3, 5}, 17},
        {170,
         {442, 525, 511, 593, 546, 564, 617},
         {41, 50, 49, 59, 55, 57, 60},
         1735},
        {750,
         {135, 139, 149, 150, 156, 163, 173, 184, 192, 201, 210, 214, 221, 229,
          240},
         {70, 73, 77, 80, 82, 87, 90, 94, 98, 106, 110, 113, 115, 118, 120},
         1458},
        {6404180,
         {825594,  1677009, 1676628, 1523970, 943972,  97426,   69666,  1296457,
          1679693, 1902996, 1844992, 1049289, 1252836, 1319836, 953277, 2067538,
          675367,  853655,  1826027, 65731,   901489,  577243,  466257, 369261},
         {382745, 799601, 909247, 729069, 467902, 44328,  34610,  698150,
          823460, 903959, 853665, 551830, 610856, 670702, 488960, 951111,
          323046, 446298, 931161, 31385,  496951, 264724, 224916, 169684},
         13549094},
    };

    for (const auto &test_case : cases) {
        CHECK(get_max_profit(test_case.profit, test_case.weight,
                             static_cast<int>(test_case.knapsack_size),
                             KnapsackStrategy::TOP_DOWN) ==
              test_case.expected_result);
    }
}

TEST_CASE("Knapsack top-down with memo implementation")
{
    const std::vector<TestCases> cases{
        {5, {4, 5, 3, 7}, {2, 3, 1, 4}, 10},
        {7, {1, 6, 10, 16}, {1, 2, 3, 5}, 22},
        {6, {1, 6, 10, 16}, {1, 2, 3, 5}, 17},
        {170,
         {442, 525, 511, 593, 546, 564, 617},
         {41, 50, 49, 59, 55, 57, 60},
         1735},
        {750,
         {135, 139, 149, 150, 156, 163, 173, 184, 192, 201, 210, 214, 221, 229,
          240},
         {70, 73, 77, 80, 82, 87, 90, 94, 98, 106, 110, 113, 115, 118, 120},
         1458},
        {6404180,
         {825594,  1677009, 1676628, 1523970, 943972,  97426,   69666,  1296457,
          1679693, 1902996, 1844992, 1049289, 1252836, 1319836, 953277, 2067538,
          675367,  853655,  1826027, 65731,   901489,  577243,  466257, 369261},
         {382745, 799601, 909247, 729069, 467902, 44328,  34610,  698150,
          823460, 903959, 853665, 551830, 610856, 670702, 488960, 951111,
          323046, 446298, 931161, 31385,  496951, 264724, 224916, 169684},
         13549094},
    };

    for (const auto &test_case : cases) {
        CHECK(get_max_profit(test_case.profit, test_case.weight,
                             static_cast<int>(test_case.knapsack_size),
                             KnapsackStrategy::TOP_DOWN_W_MEMO) ==
              test_case.expected_result);
    }
}

TEST_CASE("Knapsack bottom-up implementation")
{
    const std::vector<TestCases> cases{
        {5, {4, 5, 3, 7}, {2, 3, 1, 4}, 10},
        {7, {1, 6, 10, 16}, {1, 2, 3, 5}, 22},
        {6, {1, 6, 10, 16}, {1, 2, 3, 5}, 17},
        {170,
         {442, 525, 511, 593, 546, 564, 617},
         {41, 50, 49, 59, 55, 57, 60},
         1735},
        {750,
         {135, 139, 149, 150, 156, 163, 173, 184, 192, 201, 210, 214, 221, 229,
          240},
         {70, 73, 77, 80, 82, 87, 90, 94, 98, 106, 110, 113, 115, 118, 120},
         1458},
        {6404180,
         {825594,  1677009, 1676628, 1523970, 943972,  97426,   69666,  1296457,
          1679693, 1902996, 1844992, 1049289, 1252836, 1319836, 953277, 2067538,
          675367,  853655,  1826027, 65731,   901489,  577243,  466257, 369261},
         {382745, 799601, 909247, 729069, 467902, 44328,  34610,  698150,
          823460, 903959, 853665, 551830, 610856, 670702, 488960, 951111,
          323046, 446298, 931161, 31385,  496951, 264724, 224916, 169684},
         13549094},
    };

    for (const auto &test_case : cases) {
        CHECK(get_max_profit(test_case.profit, test_case.weight,
                             static_cast<int>(test_case.knapsack_size),
                             KnapsackStrategy::BOTTOM_UP) ==
              test_case.expected_result);
    }
}
