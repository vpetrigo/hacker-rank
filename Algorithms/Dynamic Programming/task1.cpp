#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

constexpr unsigned num_base = 10;

template <typename T>
void print_val(const std::string& s, const T& v) {
  std::cout << s << v << std::endl;
}

template <typename Cont>
void print_cont(const std::string& s, const Cont& c) {
  std::cout << s;
  std::for_each(c.cbegin(), c.cend(), [](const typename Cont::value_type& n) {
    std::cout << n;
  });
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
  c.erase(c.begin(), std::find_if_not(c.cbegin(), c.cend(), [](const typename Cont::value_type& v) { return v == 0; }));
}

// a and b must be represented in the big endian form
// example: num = 100 -> a = [1, 0, 0]
template <typename CForwardIt, typename ForwardIt>
void naive_mul(CForwardIt a_rbegin, CForwardIt a_rend, CForwardIt b_rbegin, CForwardIt b_rend,
               ForwardIt res_rbegin) {
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

template <typename CForwardIt, typename ForwardIt>
void _karatsuba_mul(CForwardIt a_rbegin, CForwardIt a_rend, CForwardIt b_rbegin, CForwardIt b_rend,
                   ForwardIt res_rbegin) {
  auto num_len = std::distance(a_rbegin, a_rend);
  if (num_len == 1) {
    naive_mul(a_rbegin, a_rend, b_rbegin, b_rend, res_rbegin);
    return;
  }
  
  _karatsuba_mul(a_rbegin, a_rbegin + num_len / 2, b_rbegin, b_rbegin + num_len / 2, res_rbegin);
  _karatsuba_mul(a_rbegin + num_len / 2, a_rend, b_rbegin + num_len / 2, b_rend, res_rbegin + num_len);
  std::vector<typename ForwardIt::value_type> partsum_a(a_rbegin, a_rbegin + num_len / 2);
  std::vector<typename ForwardIt::value_type> partsum_b(b_rbegin, b_rbegin + num_len / 2);
  std::vector<typename ForwardIt::value_type> partsum_res(num_len);
  std::transform(a_rbegin + num_len / 2, a_rend, partsum_a.rbegin(), partsum_a.rbegin(),
                 [](const typename ForwardIt::value_type& n1, const typename ForwardIt::value_type& n2) { return n1 + n2; });
  std::transform(b_rbegin + num_len / 2, b_rend, partsum_b.rbegin(), partsum_b.rbegin(),
                 [](const typename ForwardIt::value_type& n1, const typename ForwardIt::value_type& n2) { return n1 + n2; });
  _karatsuba_mul(partsum_a.crbegin(), partsum_a.crend(), partsum_b.crbegin(), partsum_b.crend(), partsum_res.rbegin());
}

template <typename Cont>
Cont karatsuba_mul(const Cont& a, const Cont& b) {
  auto a_len = a.size();
  auto b_len = b.size();
  auto max_len = std::max(a_len, b_len);
  std::size_t i = 0;
  
  while (max_len > static_cast<std::size_t> (pow(2, i))) {
    ++i;
  }
  max_len = static_cast<std::size_t> (pow(2, i));
  
  Cont ext_a(max_len);
  Cont ext_b(max_len);
  Cont res(2 * max_len);
  
  std::copy(a.crbegin(), a.crend(), ext_a.rbegin());
  std::copy(b.crbegin(), b.crend(), ext_b.rbegin());
  _karatsuba_mul(ext_a.crbegin(), ext_a.crend(), ext_b.crbegin(), ext_b.crend(), res.rbegin());
  remove_leading_zeros(res);
  
  return res;
}

template <typename Cont>
void normalize_repr(Cont& repr) {
  unsigned rem = 0;

  std::for_each(repr.rbegin(), repr.rend(), [&rem](typename Cont::value_type& n) {
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
    normalize_repr(res);
    std::swap(repr_, res);
  }
  
  void mul(const BigNum& bn) {   
    auto res = karatsuba_mul(repr_, bn.repr_);

    normalize_repr(res);
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
  }
  else {
    os << 0;
  }

  return os;
}

BigNum calc_big_modified_fib(BigNum&& a, BigNum&& b, int n) {
  BigNum f0(std::move(a));
  BigNum f1(std::move(b));
  BigNum next_f = 0;
  
  for (std::size_t i = 2; i < n; ++i) {
    next_f = f1 * f1 + f0;
    f0 = f1;
    f1 = next_f;
    print_val("i: ", i);
  }

  return f1;
}

int test() {
  BigNum a(std::string("777"));
  BigNum b(std::string("777"));
  BigNum c(std::string("000000001"));
  std::vector<int> v{0, 0,0,0,0,0,0,0,1};
  
  remove_leading_zeros(v);
  print_cont("v: ", v);
  return 0;
}

int main() {
  test();
  int a, b, n;
  std::cin >> a >> b >> n;
  std::cout << calc_big_modified_fib(a, b, n) << std::endl;

  return 0;
}
