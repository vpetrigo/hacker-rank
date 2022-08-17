/**
 * \file
 * \brief
 * \details Given an amount and the denominations of coins available,
 * determine how many ways change can be made for amount. There is a
 * limitless supply of each coin type.
 * \author Vladimir Petrigo
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

static unsigned long
impl_exchange_top_down(std::vector<unsigned>::const_iterator start,
                       const std::vector<unsigned>::const_iterator &end,
                       int value_to_change,
                       std::vector<std::vector<unsigned long long>> &memo)
{
    if (start == end) {
        return 0;
    }

    unsigned coin = *start;
    unsigned n = std::distance(start, end);

    if (value_to_change == 0) {
        memo[n][value_to_change] = 1;
    }

    if (memo[n][value_to_change] != 0) {
        return memo[n][value_to_change];
    }

    if (*start <= value_to_change) {
        memo[n][value_to_change] =
            impl_exchange_top_down(
                start, end, value_to_change - static_cast<int>(coin), memo) +
            impl_exchange_top_down(start + 1, end, value_to_change, memo);

        return memo[n][value_to_change];
    }

    memo[n][value_to_change] =
        impl_exchange_top_down(start + 1, end, value_to_change, memo);
    return memo[n][value_to_change];
}

static unsigned long long
exchange_number_calc(const std::vector<unsigned> &exchange_values,
                     int value_to_change)
{
    std::vector<std::vector<unsigned long long>> memo(
        exchange_values.size() + 1,
        std::vector<unsigned long long>(value_to_change + 1, 0));
    return impl_exchange_top_down(std::cbegin(exchange_values),
                                  std::cend(exchange_values), value_to_change,
                                  memo);
}

int main()
{
    unsigned value_to_change;
    unsigned exchange_coin_number;
    std::vector<unsigned> exchange_values;

    fscanf(stdin, "%u %u", &value_to_change, &exchange_coin_number);
    exchange_values.reserve(exchange_coin_number);
    std::generate_n(std::back_inserter(exchange_values), exchange_coin_number,
                    [] {
                        unsigned _tmp;

                        std::cin >> _tmp;

                        return _tmp;
                    });
    std::sort(std::begin(exchange_values), std::end(exchange_values));
    unsigned long long result = exchange_number_calc(
        exchange_values, static_cast<int>(value_to_change));

    std::cout << result << '\n';

    return 0;
}
