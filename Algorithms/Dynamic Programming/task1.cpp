#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <vector>

constexpr unsigned num_base = 10;

template <typename T>
std::size_t get_int_len(T num) {
  std::size_t len = 0;

  while (num) {
    num /= num_base;
    ++len;
  }

  return len;
}

void normalize_repr(std::vector<unsigned>& repr) {
  unsigned rem = 0;

  std::for_each(repr.rbegin(), repr.rend(), [&rem](unsigned& n) {
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

  dest->resize(get_int_len(num));
  std::generate(dest->rbegin(), dest->rend(), [&num_copy] {
    auto dig = num_copy % num_base;
    num_copy /= num_base;
    return dig;
  });
}

// string overloading
template <typename T, typename Cont>
void convert_to_big_num(const T& num, Cont* dest, std::false_type) {
  const auto num_len = num.size();

  if (num_len == 0) {
    dest->resize(1);
    dest->emplace_back(0);
  } 
  else {
    // don't know how to handle negative input
    assert(num[0] != '-');
    dest->resize(num_len);
    std::transform(num.crbegin(), num.crend(), dest->rbegin(),
                   [](const char ch) { return ch - '0'; });
  }
}

class BigNum {
 public:
  template <typename T>
  BigNum(const T& num) {
    static_assert(
        std::is_integral<T>::value || std::is_same<T, std::string>::value,
        "Integer or string required");
    convert_to_big_num(num, &repr_, std::is_integral<T>());
  }

  std::size_t digits() const { return repr_.size(); }

  BigNum& operator+=(const BigNum& bn) {
    std::size_t max_len = std::max(bn.digits(), digits());
    std::vector<unsigned> res(max_len + 1);

    std::transform(repr_.crbegin(), repr_.crend(), res.rbegin(),
                   [](const unsigned& n) { return n; });
    std::transform(
        bn.repr_.crbegin(), bn.repr_.crend(), res.rbegin(), res.rbegin(),
        [](const unsigned& n1, const unsigned& n2) { return n1 + n2; });
    normalize_repr(res);
    repr_ = std::move(res);

    return *this;
  }

  friend BigNum operator+(BigNum a, const BigNum& b) {
    a += b;

    return a;
  }

  BigNum& operator*=(const BigNum& bn) {
    std::size_t max_len = std::max(bn.digits(), digits());
    std::vector<unsigned> res(2 * max_len);

    for (std::size_t i = 0; i < digits(); ++i) {
      for (std::size_t j = 0; j < bn.digits(); ++j) {
        res[max_len - (i + j)] += repr_[i] * bn.repr_[j];
      }
    }
    normalize_repr(res);
    repr_ = std::move(res);

    return *this;
  }

  friend BigNum operator*(BigNum a, const BigNum& b) {
    a *= b;

    return a;
  }

  friend std::ostream& operator<<(std::ostream& os, const BigNum& bn);

 private:
  std::vector<unsigned> repr_;
};

std::ostream& operator<<(std::ostream& os, const BigNum& bn) {
  auto cit = bn.repr_.cbegin();
  auto cit_end = bn.repr_.cend();

  if ((cit = std::find_if_not(cit, cit_end, [](const unsigned& n) {
         return n == 0;
       })) != cit_end) {
    std::for_each(cit, cit_end, [&os](const unsigned& n) { os << n; });
  }
  else {
    os << 0;
  }

  return os;
}

BigNum calc_big_modified_fib(BigNum a, BigNum b, int n) {
  BigNum f0(a);
  BigNum f1(b);

  for (std::size_t i = 2; i < n; ++i) {
    BigNum next_f = f1 * f1 + f0;

    f0 = f1;
    f1 = next_f;
    std::cout << "next_f: " << next_f << std::endl;
    std::cout << "f0: " << f1 << std::endl;
    std::cout << "f1: " << f0 << std::endl;
  }

  return f1;
}

int main() {
  int a, b, n;
  std::cin >> a >> b >> n;
  BigNum ta(b);
  BigNum tc(a);
  BigNum tb = ta * ta + tc;
  std::cout << ta << std::endl;
  std::cout << tb << std::endl;
  std::cout << ta << std::endl;
  auto res = calc_big_modified_fib(a, b, n);
  std::cout << res << std::endl;

  return 0;
}
