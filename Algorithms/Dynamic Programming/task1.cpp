#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

// Big Integer base
constexpr unsigned num_base = 10;

// Printers helper functions
// Print variables @v after an informational string @s
template <typename... Ts>
void print_val(const std::string& s, const Ts&... v);
// Print container @c after an informational string @s
template <typename ForwardIt>
void print_cont(const std::string& s, ForwardIt begin, ForwardIt end);

// Numeric helper functions
template <typename T>
std::size_t get_int_len(T num);
// remove leading zeros in an internal Big Integer container
template <typename Cont>
void remove_leading_zeros(Cont& c);

// a and b must be represented in the big endian form
// example: num = 100 -> a = [1, 0, 0]
template <typename CForwardIt, typename ForwardIt>
void naive_mul(CForwardIt a_rbegin, CForwardIt a_rend, CForwardIt b_rbegin,
               CForwardIt b_rend, ForwardIt res_rbegin);

template <typename Cont>
Cont karatsuba_mul(const Cont& a, const Cont& b);

template <typename CForwardIt, typename ForwardIt>
void _karatsuba_mul(CForwardIt a_rbegin, CForwardIt a_rend, CForwardIt b_rbegin,
                    CForwardIt b_rend, ForwardIt res_rbegin);

template <typename ForwardIt>
void normalize_repr(ForwardIt repr_begin, ForwardIt repr_end);

// Conversion input to Big Integer
// integer overloading
template <typename T, typename Cont>
void convert_to_big_num(const T& num, Cont* dest, std::true_type);

// string overloading
template <typename T, typename Cont>
void convert_to_big_num(const T& num, Cont* dest, std::false_type);

class BigNum {
 public:
  using container = std::vector<unsigned long>;
  using value_type = typename container::value_type;

  template <typename T>
  BigNum(const T& num) {
    static_assert(
        std::is_integral<T>::value || std::is_same<T, std::string>::value,
        "Integer or string required");
    convert_to_big_num(num, &repr_, std::is_integral<T>());
  }

  std::size_t digits() const { return repr_.size(); }

  void add(const BigNum& bn) {
    std::size_t max_len = std::max(bn.digits(), digits());
    container res(max_len + 1);

    std::copy(repr_.crbegin(), repr_.crend(), res.rbegin());
    std::transform(
        bn.repr_.crbegin(), bn.repr_.crend(), res.rbegin(), res.rbegin(),
        [](const value_type& n1, const value_type& n2) { return n1 + n2; });
    normalize_repr(res.rbegin(), res.rend());
    remove_leading_zeros(res);
    std::swap(repr_, res);
  }

  void mul(const BigNum& bn) {
    auto res = karatsuba_mul(repr_, bn.repr_);

    std::swap(repr_, res);
  }

  BigNum& operator+=(const BigNum& bn) {
    add(bn);

    return *this;
  }

  friend BigNum operator+(BigNum a, const BigNum& b) {
    a += b;

    return a;
  }

  BigNum& operator*=(const BigNum& bn) {
    mul(bn);

    return *this;
  }

  friend BigNum operator*(BigNum a, const BigNum& b) {
    a *= b;

    return a;
  }

  friend std::ostream& operator<<(std::ostream& os, const BigNum& bn);

 private:
  container repr_;
};

std::ostream& operator<<(std::ostream& os, const BigNum& bn) {
  auto cit = bn.repr_.cbegin();
  auto cit_end = bn.repr_.cend();

  if ((cit = std::find_if_not(cit, cit_end, [](const unsigned& n) {
         return n == 0;
       })) != cit_end) {
    std::for_each(cit, cit_end, [&os](const unsigned& n) { os << n; });
  } else {
    os << 0;
  }

  return os;
}

// calculate the n-th modified Fibonacci number
BigNum calc_big_modified_fib(BigNum&& a, BigNum&& b, int n);

int main() {
  int a, b, n;
  std::cin >> a >> b >> n;
  // calculate modified Fib numbers: F(i + 2) = F(i + 1)^2 + F(i)
  auto res = calc_big_modified_fib(a, b, n);
  std::cout << res << std::endl;

  return 0;
}

template <typename... Ts>
void print_val(const std::string& s, const Ts&... v) {
  std::cout << s;
  using extender = int[];
  (void)extender{(std::cout << v << std::endl, 0)...};
}

template <typename ForwardIt>
void print_cont(const std::string& s, ForwardIt begin, ForwardIt end) {
  using T = typename ForwardIt::value_type;
  std::cout << s;
  std::for_each(begin, end, [](const T& n) { std::cout << n; });
  std::cout << std::endl;
}

template <typename T>
std::size_t get_int_len(T num) {
  std::size_t len = 0;

  while (num) {
    num /= num_base;
    ++len;
  }

  return len;
}

template <typename Cont>
void remove_leading_zeros(Cont& c) {
  c.erase(c.begin(), std::find_if_not(c.cbegin(), c.cend(),
                                      [](const typename Cont::value_type& v) {
                                        return v == 0;
                                      }));
}

template <typename CForwardIt, typename ForwardIt>
void naive_mul(CForwardIt a_rbegin, CForwardIt a_rend, CForwardIt b_rbegin,
               CForwardIt b_rend, ForwardIt res_rbegin) {
  // mul_offset determine an offset on each step of multiplication
  // Example:         11
  //                  12
  //                  --
  //               + -22 <- mul_offset = 0
  //                 11- <- mul_offset = 1
  std::size_t mul_offset = 0;

  for (auto ait = a_rbegin; ait != a_rend; ++ait) {
    std::size_t inc = 0;
    for (auto bit = b_rbegin; bit != b_rend; ++bit) {
      *(res_rbegin + mul_offset + inc) += *ait * *bit;
      ++inc;
    }
    ++mul_offset;
  }
}

template <typename Cont>
Cont karatsuba_mul(const Cont& a, const Cont& b) {
  auto a_len = a.size();
  auto b_len = b.size();
  auto max_len = std::max(a_len, b_len);
  std::size_t i = 0;
  // make length the degree of 2
  max_len =
      static_cast<std::size_t>(std::pow(2, std::ceil(std::log2(max_len) + 1)));

  Cont ext_a(max_len);
  Cont ext_b(max_len);
  Cont res(2 * max_len);

  std::copy(a.crbegin(), a.crend(), ext_a.rbegin());
  std::copy(b.crbegin(), b.crend(), ext_b.rbegin());
  _karatsuba_mul(ext_a.crbegin(), ext_a.crend(), ext_b.crbegin(), ext_b.crend(),
                 res.rbegin());
  normalize_repr(res.rbegin(), res.rend());
  remove_leading_zeros(res);

  return res;
}

template <typename CForwardIt, typename ForwardIt>
void _karatsuba_mul(CForwardIt a_rbegin, CForwardIt a_rend, CForwardIt b_rbegin,
                    CForwardIt b_rend, ForwardIt res_rbegin) {
  constexpr auto LEN_FOR_NAIVE_MUL = 32;
  auto num_len = std::distance(a_rbegin, a_rend);
  auto res_mid = res_rbegin + num_len;
  auto res_end = res_mid + num_len;

  if (num_len <= LEN_FOR_NAIVE_MUL) {
    naive_mul(a_rbegin, a_rend, b_rbegin, b_rend, res_rbegin);
    return;
  }
  // handy aliases
  auto a_mid = a_rbegin + num_len / 2;
  auto b_mid = b_rbegin + num_len / 2;
  using T = typename ForwardIt::value_type;
  using PartCont = std::vector<typename ForwardIt::value_type>;
  // P2 = Xr * Yr
  _karatsuba_mul(a_rbegin, a_mid, b_rbegin, b_mid, res_rbegin);
  // P1 = Xl * Yl
  _karatsuba_mul(a_mid, a_rend, b_mid, b_rend, res_rbegin + num_len);
  // storage for Xr + Xl; store Xr
  PartCont partsum_a(num_len / 2);
  // storage for Yr + Yl; store Xr
  PartCont partsum_b(num_len / 2);
  // storage for P3 = (Xr + Xl) * (Yr + Yl)
  PartCont partsum_res(num_len);
  // add Xl to @partsum_a
  std::transform(a_mid, a_rend, a_rbegin, partsum_a.rbegin(),
                 [](const T& n1, const T& n2) { return n1 + n2; });
  // add Yl to @partsum_b
  std::transform(b_mid, b_rend, b_rbegin, partsum_b.rbegin(),
                 [](const T& n1, const T& n2) { return n1 + n2; });
  // P3 = (Xr + Xl) * (Yr + Yl)
  _karatsuba_mul(partsum_a.crbegin(), partsum_a.crend(), partsum_b.crbegin(),
                 partsum_b.crend(), partsum_res.rbegin());
  // After all we have to calculate P3 - P1 - P2
  // P3 - P1
  std::transform(partsum_res.rbegin(), partsum_res.rend(), res_rbegin,
                 partsum_res.rbegin(),
                 [](const T& n1, const T& n2) { return n1 - n2; });
  // P3 - P2
  std::transform(partsum_res.rbegin(), partsum_res.rend(), res_mid,
                 partsum_res.rbegin(),
                 [](const T& n1, const T& n2) { return n1 - n2; });
  // insert P3 into the middle of the result
  std::transform(partsum_res.crbegin(), partsum_res.crend(),
                 res_rbegin + num_len / 2, res_rbegin + num_len / 2,
                 [](const T& n1, const T& n2) { return n1 + n2; });
}

template <typename ForwardIt>
void normalize_repr(ForwardIt repr_begin, ForwardIt repr_end) {
  using T = typename ForwardIt::value_type;
  T rem = 0;

  std::for_each(repr_begin, repr_end, [&rem](T& n) {
    n += rem;
    rem = n / num_base;
    n %= num_base;
  });
}

// integer overloading
template <typename T, typename Cont>
void convert_to_big_num(const T& num, Cont* dest, std::true_type) {
  assert(num >= 0);
  auto num_copy = num;
  auto digits = get_int_len(num);

  dest->resize(digits ? digits : 0);
  std::generate(dest->rbegin(), dest->rend(), [&num_copy] {
    auto dig = num_copy % num_base;
    num_copy /= num_base;
    return dig;
  });
}

// string overloading
template <typename T, typename Cont>
void convert_to_big_num(const T& num, Cont* dest, std::false_type) {
  const auto num_len =
      std::distance(std::find_if_not(num.cbegin(), num.cend(),
                                     [](char ch) { return ch == '0'; }),
                    num.cend());

  if (num_len == 0) {
    dest->resize(1);
    dest->emplace_back(0);
  } else {
    // don't know how to handle negative input
    assert(num[0] != '-');
    dest->resize(num_len);
    std::transform(num.crbegin(), num.crend(), dest->rbegin(),
                   [](const char ch) { return ch - '0'; });
  }
}

BigNum calc_big_modified_fib(BigNum&& a, BigNum&& b, int n) {
  BigNum f0(std::move(a));
  BigNum f1(std::move(b));
  BigNum next_f = 0;

  for (std::size_t i = 2; i < n; ++i) {
    next_f = f1 * f1 + f0;
    f0 = f1;
    f1 = next_f;
  }

  return f1;
}
