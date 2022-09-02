/**
 * \file
 * \brief
 * \details We define a magic square to be an  matrix of distinct
 * positive integers from  to  where the sum of any row, column, or
 * diagonal of length  is always equal to the same number: the magic
 * constant.
 * \author Vladimir Petrigo
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <numeric>
#include <valarray>

constexpr unsigned SQUARE_SIZE = 3;

constexpr unsigned magic_constant_calc(std::size_t magic_square_size) noexcept {
    return (magic_square_size * magic_square_size + 1) / 2 * magic_square_size;
}

class Matrix {
public:
    Matrix(std::size_t dim, std::initializer_list<int> il) noexcept
        : dim_{dim}, data_{il} {
    }

    Matrix(std::size_t dim, std::valarray<int> arr) noexcept
        : dim_{dim}, data_{std::move(arr)} {
    }

    unsigned column_sum(std::size_t col) const noexcept {
        return data_[std::slice(col, dim_, dim_)].sum();
    }

    unsigned row_sum(std::size_t row) const noexcept {
        return data_[std::slice(dim_ * row, dim_, 1)].sum();
    }

    unsigned diagonal_sum() const noexcept {
        return data_[std::slice(0, dim_, dim_ + 1)].sum();
    }

    unsigned secondary_diagonal_sum() const noexcept {
        return data_[std::slice(dim_ - 1, dim_, dim_ - 1)].sum();
    }

private:
    const std::size_t dim_;
    std::valarray<int> data_;
};

bool is_magic(const Matrix &m) noexcept {
    if (m.diagonal_sum() != magic_constant_calc(SQUARE_SIZE)) {
        return false;
    }

    if (m.secondary_diagonal_sum() != magic_constant_calc(SQUARE_SIZE)) {
        return false;
    }

    for (size_t i = 0; i < SQUARE_SIZE; ++i) {
        if (m.column_sum(i) != magic_constant_calc(SQUARE_SIZE) ||
            m.row_sum(i) != magic_constant_calc(SQUARE_SIZE)) {
            return false;
        }
    }

    return true;
}

static std::pair<std::valarray<int>, bool> find_next_magic_square() noexcept {
    static std::valarray<int> init{1, 2, 3, 4, 5, 6, 7, 8, 9};

    while (std::next_permutation(std::begin(init), std::end(init))) {
        const auto magic_matrix = Matrix(SQUARE_SIZE, init);

        if (is_magic(magic_matrix)) {
            return std::make_pair(init, true);
        }
    }

    return std::make_pair(std::valarray<int>{}, false);
}

void print_matrix(const std::valarray<int> &arr) noexcept {
    std::for_each(std::begin(arr), std::end(arr),
                  [](const auto &e) { std::cout << e << ' '; });
    std::cout << '\n';
}

int main() {
    std::valarray<int> matrix_repr(SQUARE_SIZE * SQUARE_SIZE);

    for (size_t i = 0; i < SQUARE_SIZE; ++i) {
        fscanf(stdin, "%u %u %u", &matrix_repr[i * SQUARE_SIZE],
               &matrix_repr[i * SQUARE_SIZE + 1],
               &matrix_repr[i * SQUARE_SIZE + 2]);
    }

    int cost{std::numeric_limits<int>::max()};

    do {
        const auto v = find_next_magic_square();

        if (!v.second) {
            break;
        }

        const auto result = (matrix_repr - v.first).apply(std::abs).sum();
        cost = std::min(cost, result);
    } while (true);

    std::cout << std::abs(cost) << '\n';

    return 0;
}
