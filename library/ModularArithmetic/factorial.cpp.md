---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :heavy_check_mark: 階乗の高速計算 <small>(ModularArithmetic/factorial.cpp)</small>

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#495e431c85de4c533fce4ff12db613fe">ModularArithmetic</a>
* <a href="{{ site.github.repository_url }}/blob/master/ModularArithmetic/factorial.cpp">View this file on GitHub</a>
    - Last commit date: 2020-04-10 16:57:52+09:00




## Depends on

* :heavy_check_mark: <a href="garner.cpp.html">Garner's algorithm <small>(ModularArithmetic/garner.cpp)</small></a>
* :heavy_check_mark: <a href="modint.cpp.html">合同算術用クラス <small>(ModularArithmetic/modint.cpp)</small></a>
* :heavy_check_mark: <a href="modtable.cpp.html">合同演算の前計算テーブル <small>(ModularArithmetic/modtable.cpp)</small></a>
* :heavy_check_mark: <a href="polynomial.cpp.html">多項式 <small>(ModularArithmetic/polynomial.cpp)</small></a>
* :heavy_check_mark: <a href="../integer/bit.cpp.html">ビット演算 <small>(integer/bit.cpp)</small></a>
* :heavy_check_mark: <a href="../integer/sqrt.cpp.html">整数の平方根 <small>(integer/sqrt.cpp)</small></a>


## Verified with

* :heavy_check_mark: <a href="../../verify/test/yc_502.test.cpp.html">test/yc_502.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#ifndef H_factorial
#define H_factorial

/**
 * @brief 階乗の高速計算
 * @author えびちゃん
 */

#include <algorithm>
#include <vector>

#include "ModularArithmetic/modint.cpp"
#include "ModularArithmetic/modtable.cpp"
#include "ModularArithmetic/polynomial.cpp"
#include "integer/sqrt.cpp"
#include "integer/bit.cpp"

template <intmax_t Mod>
modint<Mod> factorial(intmax_t n) {
  if (n == 0) return 1;
  if (n >= Mod) return 0;
  intmax_t v = ceil2(isqrt(n));
  using value_type = modint<Mod>;
  modtable<value_type> mt(v);

  std::vector<value_type> g{1, v+1};
  for (intmax_t d = 1; d < v; d <<= 1) {
    std::vector<value_type> a(d+1);
    for (intmax_t i = 0; i <= d; ++i) {
      a[i] = g[i] * mt.factorial_inverse(i) * mt.factorial_inverse(d-i);
      if ((d-i) % 2 != 0) a[i] = -a[i];
    }

    auto shift = [&](intmax_t m0) {
      m0 = (value_type(m0) / v).get();
      intmax_t m = std::max(m0, d+1);

      std::vector<value_type> b(d+1), c(d+1);
      for (intmax_t i = 0; i <= d; ++i) {
        b[i] = value_type{1} / (m+i);
        c[d-i] = value_type{1} / (m-i-1);
      }

      polynomial<value_type> fa(a.begin(), a.end()), fb(b.begin(), b.end()), fc(c.begin(), c.end());
      fb *= fa, fc *= fa;

      std::vector<value_type> res(d+1);
      value_type mul = 1;
      for (int j = 0; j <= d; ++j) mul *= m-j;
      for (int k = 0; k <= d; ++k) {
        res[k] = (fb[k] + fc[k+d+1]) * mul;
        if (k < d) mul *= value_type(m+k+1) / value_type(m+k-d);
      }
      if (m0 <= d) {
        for (intmax_t i = m0; i--;) res[d-(m0-i-1)] = res[i];
        for (intmax_t i = m0; i <= d; ++i) res[i-m0] = g[i];
      }
      return res;
    };
    std::vector<value_type> gd = shift(d);
    std::vector<value_type> gdv = shift(d*v);
    std::vector<value_type> gdvd = shift(d*v+d);

    g.resize(2*d+1);
    for (intmax_t i = 0; i < d; ++i) g[i] *= gd[i];
    for (intmax_t i = 0; i <= d; ++i) g[d+i] = gdv[i] * gdvd[i];
  }

  value_type res = 1;
  intmax_t i = 0;
  for (size_t j = 0; i+v <= n && j < g.size(); i += v, ++j) res *= g[j];
  for (++i; i <= n; ++i) res *= i;
  return res;
}

#endif  /* !defined(H_factorial) */

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "ModularArithmetic/factorial.cpp"



/**
 * @brief 階乗の高速計算
 * @author えびちゃん
 */

#include <algorithm>
#include <vector>

#line 1 "ModularArithmetic/modint.cpp"



/**
 * @brief 合同算術用クラス
 * @author えびちゃん
 */
//

#include <cstdint>
#include <type_traits>
#include <utility>

template <intmax_t Modulo>
class modint {
public:
  using value_type = intmax_t;

private:
  static constexpr value_type S_cmod = Modulo;  // compile-time
  static value_type S_rmod;  // runtime
  value_type M_value = 0;

  static constexpr value_type S_inv(value_type n, value_type m) {
    value_type x = 0;
    value_type y = 1;
    value_type a = n;
    value_type b = m;
    for (value_type u = y, v = x; a;) {
      value_type q = b / a;
      std::swap(x -= q*u, u);
      std::swap(y -= q*v, v);
      std::swap(b -= q*a, a);
    }
    if ((x %= m) < 0) x += m;
    return x;
  }

  static value_type S_normalize(value_type n, value_type m) {
    if (n >= m) {
      n %= m;
    } else if (n < 0) {
      if ((n %= m) < 0) n += m;
    }
    return n;
  }

public:
  modint() = default;
  modint(value_type n): M_value(S_normalize(n, get_modulo())) {}

  modint& operator =(value_type n) {
    M_value = S_normalize(n, get_modulo());
    return *this;
  }

  modint& operator +=(modint const& that) {
    if ((M_value += that.M_value) >= get_modulo()) M_value -= get_modulo();
    return *this;
  }
  modint& operator -=(modint const& that) {
    if ((M_value -= that.M_value) < 0) M_value += get_modulo();
    return *this;
  }
  modint& operator *=(modint const& that) {
    (M_value *= that.M_value) %= get_modulo();
    return *this;
  }
  modint& operator /=(modint const& that) {
    (M_value *= S_inv(that.M_value, get_modulo())) %= get_modulo();
    return *this;
  }

  modint operator +(modint const& that) const { return modint(*this) += that; }
  modint operator -(modint const& that) const { return modint(*this) -= that; }
  modint operator *(modint const& that) const { return modint(*this) *= that; }
  modint operator /(modint const& that) const { return modint(*this) /= that; }

  modint operator +() const { return *this; }
  modint operator -() const {
    if (M_value == 0) return *this;
    return modint(get_modulo() - M_value);
  }

  bool operator ==(modint const& that) const { return M_value == that.M_value; }
  bool operator !=(modint const& that) const { return !(*this == that); }

  value_type get() const { return M_value; }
  static value_type get_modulo() { return ((S_cmod > 0)? S_cmod: S_rmod); }

  template <int M = Modulo, typename Tp = typename std::enable_if<(M <= 0)>::type>
  static Tp set_modulo(value_type m) { S_rmod = m; }
};

template <typename Tp, intmax_t Modulo>
modint<Modulo> operator +(Tp const& lhs, modint<Modulo> const& rhs) {
  return rhs + lhs;
}
template <typename Tp, intmax_t Modulo>
modint<Modulo> operator -(Tp const& lhs, modint<Modulo> const& rhs) {
  return -(rhs - lhs);
}
template <typename Tp, intmax_t Modulo>
modint<Modulo> operator *(Tp const& lhs, modint<Modulo> const& rhs) {
  return rhs * lhs;
}
template <typename Tp, intmax_t Modulo>
modint<Modulo> operator /(Tp const& lhs, modint<Modulo> const& rhs) {
  return modint<Modulo>(lhs) / rhs;
}
template <typename Tp, intmax_t Modulo>
bool operator ==(Tp const& lhs, modint<Modulo> const& rhs) {
  return rhs == lhs;
}
template <typename Tp, intmax_t Modulo>
bool operator !=(Tp const& lhs, modint<Modulo> const& rhs) {
  return !(lhs == rhs);
}

template <intmax_t N>
constexpr intmax_t modint<N>::S_cmod;
template <intmax_t N>
intmax_t modint<N>::S_rmod;


#line 1 "ModularArithmetic/modtable.cpp"



/**
 * @brief 合同演算の前計算テーブル
 * @author えびちゃん
 */

#include <cstddef>
#line 11 "ModularArithmetic/modtable.cpp"

template <typename ModInt>
class modtable {
public:
  using value_type = ModInt;
  using size_type = size_t;
  using underlying_type = typename ModInt::value_type;

private:
  std::vector<value_type> M_f, M_i, M_fi;

public:
  modtable() = default;

  explicit modtable(underlying_type n): M_f(n+1), M_i(n+1), M_fi(n+1) {
    M_f[0] = 1;
    for (underlying_type i = 1; i <= n; ++i)
      M_f[i] = M_f[i-1] * i;

    underlying_type mod = M_f[0].get_modulo();
    M_i[1] = 1;
    for (underlying_type i = 2; i <= n; ++i)
      M_i[i] = -value_type(mod / i) * M_i[mod % i];

    M_fi[0] = 1;
    for (underlying_type i = 1; i <= n; ++i)
      M_fi[i] = M_fi[i-1] * M_i[i];
  }

  value_type inverse(underlying_type n) const { return M_i[n]; }
  value_type factorial(underlying_type n) const { return M_f[n]; }
  value_type factorial_inverse(underlying_type n) const { return M_fi[n]; }

  value_type binom(underlying_type n, underlying_type k) const {
    if (n < 0 || n < k || k < 0) return 0;
    // assumes n < mod
    return M_f[n] * M_fi[k] * M_fi[n-k];
  }
};


#line 1 "ModularArithmetic/polynomial.cpp"



/**
 * @brief 多項式
 * @author えびちゃん
 */

#line 10 "ModularArithmetic/polynomial.cpp"
#include <climits>
#line 13 "ModularArithmetic/polynomial.cpp"

#line 1 "integer/bit.cpp"



/** 
 * @brief ビット演算
 * @author えびちゃん
 */

// XXX integral promotion 関連の注意をあまりしていません

#line 12 "integer/bit.cpp"
#include <type_traits>

template <typename Tp>
constexpr auto countl_zero(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, int>::type
{
  using value_type = typename std::make_unsigned<Tp>::type;
  int bits = (sizeof(value_type) * CHAR_BIT);
  if (n == 0) return bits;
  int res = 0;
  for (int i = bits / 2; i > 0; i /= 2) {
    value_type mask = ((static_cast<value_type>(1) << i) - 1) << i;
    if (n & mask) n >>= i;
    else res += i;
  }
  return res;
}
template <typename Tp>
constexpr auto countl_one(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, int>::type
{
  using value_type = typename std::make_unsigned<Tp>::type;
  return countl_zero(static_cast<value_type>(~n));
}

template <typename Tp>
constexpr auto countr_zero(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, int>::type
{
  using value_type = typename std::make_unsigned<Tp>::type;
  int bits = (sizeof(value_type) * CHAR_BIT);
  if (n == 0) return bits;
  int res = 0;
  for (int i = bits / 2; i > 0; i /= 2) {
    value_type mask = ((static_cast<value_type>(1) << i) - 1);
    if (!(n & mask)) res += i, n >>= i;
  }
  return res;
}
template <typename Tp>
constexpr auto countr_one(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, int>::type
{
  using value_type = typename std::make_unsigned<Tp>::type;
  return countr_zero(static_cast<value_type>(~n));
}

constexpr unsigned long long half_mask[] = {
  0x5555555555555555uLL, 0x3333333333333333uLL, 0x0F0F0F0F0F0F0F0FuLL,
  0x00FF00FF00FF00FFuLL, 0x0000FFFF0000FFFFuLL, 0x00000000FFFFFFFFuLL
};

template <typename Tp>
constexpr auto popcount(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, int>::type
{
  int bits = static_cast<int>((sizeof n) * CHAR_BIT);
  for (int i = 0, j = 1; j < bits; ++i, j *= 2) {
    if (j <= 8) n = (n & half_mask[i]) + ((n >> j) & half_mask[i]);
    else n += n >> j;
  }
  return n & 0xFF;
}

template <typename Tp>
constexpr auto parity(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, int>::type
{ return popcount(n) & 1; }


template <typename Tp>
int clz(Tp n) { return countl_zero(static_cast<typename std::make_unsigned<Tp>::type>(n)); }
template <typename Tp>
int ctz(Tp n) { return countr_zero(static_cast<typename std::make_unsigned<Tp>::type>(n)); }

template <typename Tp>
int ilog2(Tp n) {
  return (CHAR_BIT * sizeof(Tp) - 1) - clz(static_cast<typename std::make_unsigned<Tp>::type>(n));
}
template <typename Tp>
bool is_pow2(Tp n) { return (n > 0) && ((n & (n-1)) == 0); }
template <typename Tp>
Tp floor2(Tp n) { return is_pow2(n)? n: static_cast<Tp>(1) << ilog2(n); }
template <typename Tp>
Tp ceil2(Tp n) { return is_pow2(n)? n: static_cast<Tp>(2) << ilog2(n); }

template <typename Tp>
constexpr auto reverse(Tp n)
  -> typename std::enable_if<std::is_unsigned<Tp>::value, Tp>::type
{
  int bits = static_cast<int>((sizeof n) * CHAR_BIT);
  for (int i = 0, j = 1; j < bits; ++i, j *= 2) {
    n = ((n & half_mask[i]) << j) | ((n >> j) & half_mask[i]);
  }
  return n;
}


#line 1 "ModularArithmetic/garner.cpp"



/**
 * @brief Garner's algorithm
 * @author えびちゃん
 */

#line 10 "ModularArithmetic/garner.cpp"
#include <tuple>
#line 12 "ModularArithmetic/garner.cpp"

class simultaneous_congruences_garner {
public:
  using value_type = intmax_t;
  using size_type = size_t;

private:
  value_type M_mod = 1;
  value_type M_sol = 0;
  std::vector<value_type> M_c, M_m;

  static auto S_gcd_bezout(value_type a, value_type b) {
    value_type x{1}, y{0};
    for (value_type u{y}, v{x}; b != 0;) {
      value_type q{a/b};
      std::swap(x -= q*u, u);
      std::swap(y -= q*v, v);
      std::swap(a -= q*b, b);
    }
    return std::make_tuple(a, x, y);
  }

public:
  simultaneous_congruences_garner() = default;

  void push(value_type a, value_type m) {
    if (M_c.empty()) {
      M_c.push_back(a);
      M_m.push_back(m);
      return;
    }

    value_type c = M_c.back();
    value_type mi = M_m.back();
    for (size_type i = M_c.size()-1; i--;) {
      c = (c * M_m[i] + M_c[i]) % m;
      (mi *= M_m[i]) %= m;
    }
    c = (a-c) * std::get<1>(S_gcd_bezout(mi, m)) % m;
    if (c < 0) c += m;
    M_c.push_back(c);
    M_m.push_back(m);
  }

  auto get(value_type m) const {
    value_type x = M_c.back() % m;
    for (size_type i = M_c.size()-1; i--;) {
      x = (x * M_m[i] + M_c[i]) % m;
    }
    return x;
  }
};


#line 17 "ModularArithmetic/polynomial.cpp"

template <typename ModInt>
class polynomial {
public:
  using size_type = size_t;
  using value_type = ModInt;

private:
  std::vector<value_type> M_f;

  void M_normalize() {
    while (!M_f.empty() && M_f.back() == 0) M_f.pop_back();
  }

  static value_type S_omega() {
    auto p = value_type{}.get_modulo();
    // for p = (u * 2**e + 1) with generator g, returns g ** u mod p
    if (p == 998244353 /* (119 << 23 | 1 */) return 15311432;  // g = 3
    if (p == 163577857 /* (39 << 22 | 1) */) return 121532577;  // g = 23
    if (p == 167772161 /* (5 << 25 | 1) */) return 243;  // g = 3
    if (p == 469762049 /* (7 << 26 | 1) */) return 2187;  // g = 3

    return 0;  // XXX
  }

  void M_fft(bool inverse = false) {
    size_type il = ilog2(M_f.size());
    for (size_type i = 1; i < M_f.size(); ++i) {
      size_type j = reverse(i) >> ((CHAR_BIT * sizeof(size_type)) - il);
      if (i < j) std::swap(M_f[i], M_f[j]);
    }

    size_type zn = ctz(M_f[0].get_modulo()-1);
    // pow_omega[i] = omega ^ (2^i)
    std::vector<value_type> pow_omega(zn+1, 1);
    pow_omega[0] = S_omega();
    if (inverse) pow_omega[0] = 1 / pow_omega[0];
    for (size_type i = 1; i < pow_omega.size(); ++i)
      pow_omega[i] = pow_omega[i-1] * pow_omega[i-1];

    for (size_type i = 1, ii = zn-1; i < M_f.size(); i <<= 1, --ii) {
      value_type omega(1);
      for (size_type jl = 0, jr = i; jr < M_f.size();) {
        auto x = M_f[jl];
        auto y = M_f[jr] * omega;
        M_f[jl] = x + y;
        M_f[jr] = x - y;
        ++jl, ++jr;
        if ((jl & i) == i) {
          jl += i, jr += i, omega = 1;
        } else {
          omega *= pow_omega[ii];
        }
      }
    }

    if (inverse) {
      value_type n1 = value_type(1) / M_f.size();
      for (auto& c: M_f) c *= n1;
    }
  }
  void M_ifft() { M_fft(true); }

  void M_naive_multiplication(polynomial const& that) {
    size_type deg = M_f.size() + that.M_f.size() - 1;
    std::vector<value_type> res(deg, 0);
    for (size_type i = 0; i < M_f.size(); ++i)
      for (size_type j = 0; j < that.M_f.size(); ++j)
        res[i+j] += M_f[i] * that.M_f[j];
    M_f = std::move(res);
    M_normalize();
  }

  void M_naive_division(polynomial that) {
    size_type deg = M_f.size() - that.M_f.size();
    std::vector<value_type> res(deg+1);
    for (size_type i = deg+1; i--;) {
      value_type c = M_f[that.M_f.size()+i-1] / that.M_f.back();
      res[i] = c;
      for (size_type j = 0; j < that.M_f.size(); ++j)
        M_f[that.M_f.size()+i-j-1] -= c * that.M_f[that.M_f.size()-j-1];
    }
    M_f = std::move(res);
    M_normalize();
  }

  void M_arbitrary_modulo_convolve(polynomial that) {
    size_type n = M_f.size() + that.M_f.size() - 1;
    std::vector<intmax_t> f(n, 0), g(n, 0);
    for (size_type i = 0; i < M_f.size(); ++i) f[i] = M_f[i].get();
    for (size_type i = 0; i < that.M_f.size(); ++i) g[i] = that.M_f[i].get();
    polynomial<modint<998244353>> f1(f.begin(), f.end()), g1(g.begin(), g.end());
    polynomial<modint<163577857>> f2(f.begin(), f.end()), g2(g.begin(), g.end());
    polynomial<modint<167772161>> f3(f.begin(), f.end()), g3(g.begin(), g.end());

    f1 *= g1;
    f2 *= g2;
    f3 *= g3;
    M_f.resize(n);
    for (size_type i = 0; i < n; ++i) {
      simultaneous_congruences_garner scg;
      scg.push(f1[i].get(), 998244353);
      scg.push(f2[i].get(), 163577857);
      scg.push(f3[i].get(), 167772161);
      M_f[i] = scg.get(value_type::get_modulo());
    }
    M_normalize();
  }

  polynomial(size_type n, value_type x): M_f(n, x) {}  // not normalized

public:
  polynomial() = default;

  template <typename InputIt>
  polynomial(InputIt first, InputIt last): M_f(first, last) { M_normalize(); }
  polynomial(std::initializer_list<value_type> il): polynomial(il.begin(), il.end()) {}

  polynomial inverse(size_type m) const {
    // XXX only for friendly moduli
    polynomial res{1 / M_f[0]};
    for (size_type d = 1; d < m; d *= 2) {
      polynomial f(d+d, 0), g(d+d, 0);
      for (size_type j = 0; j < d+d; ++j) f.M_f[j] = (*this)[j];
      for (size_type j = 0; j < d; ++j) g.M_f[j] = res.M_f[j];

      f.M_fft();
      g.M_fft();
      for (size_type j = 0; j < d+d; ++j) f.M_f[j] *= g.M_f[j];
      f.M_ifft();
      for (size_type j = 0; j < d; ++j) {
        f.M_f[j] = 0;
        f.M_f[j+d] = -f.M_f[j+d];
      }
      f.M_fft();
      for (size_type j = 0; j < d+d; ++j) f.M_f[j] *= g.M_f[j];
      f.M_ifft();
      for (size_type j = 0; j < d; ++j) f.M_f[j] = res.M_f[j];
      res = std::move(f);
    }
    res.M_f.erase(res.M_f.begin()+m, res.M_f.end());
    return res;
  }

  std::vector<value_type> multipoint_evaluate(std::vector<value_type> const& xs) const {
    size_type m = xs.size();
    size_type m2 = ceil2(m);
    std::vector<polynomial> g(m2+m2, {1});
    for (size_type i = 0; i < m; ++i) g[m2+i] = {-xs[i], 1};
    for (size_type i = m2; i-- > 1;) g[i] = g[i<<1|0] * g[i<<1|1];

    g[1] = (*this) % g[1];
    for (size_type i = 2; i < m2+m; ++i) g[i] = g[i>>1] % g[i];
    std::vector<value_type> ys(m);
    for (size_type i = 0; i < m; ++i) ys[i] = g[m2+i][0];
    return ys;
  }

  void differentiate() {
    for (size_type i = 0; i+1 < M_f.size(); ++i) M_f[i] = (i+1) * M_f[i+1];
    if (!M_f.empty()) M_f.pop_back();
  }

  void integrate(value_type c = 0) {
    // for (size_type i = 0; i < M_f.size(); ++i) M_f[i] /= i+1;
    size_type n = M_f.size();
    std::vector<value_type> inv(n+1, 1);
    auto mod = value_type::get_modulo();
    for (size_type i = 2; i <= n; ++i)
      inv[i] = -value_type(mod / i).get() * inv[mod % i];
    for (size_type i = 0; i < n; ++i) M_f[i] *= inv[i+1];
    if (!(c == 0 && M_f.empty())) M_f.insert(M_f.begin(), c);
  }

  polynomial& operator +=(polynomial const& that) {
    if (M_f.size() < that.M_f.size())
      M_f.resize(that.M_f.size(), 0);
    for (size_type i = 0; i < that.M_f.size(); ++i)
      M_f[i] += that.M_f[i];
    M_normalize();
    return *this;
  }

  polynomial& operator -=(polynomial const& that) {
    if (M_f.size() < that.M_f.size())
      M_f.resize(that.M_f.size(), 0);
    for (size_type i = 0; i < that.M_f.size(); ++i)
      M_f[i] -= that.M_f[i];
    M_normalize();
    return *this;
  }

  polynomial& operator *=(polynomial that) {
    if (zero() || that.zero()) {
      M_f.clear();
      return *this;
    }
    if (that.M_f.size() == 1) {
      // scalar multiplication
      auto m = that.M_f[0];
      for (auto& c: M_f) c *= m;
      return *this;
    }
    if (M_f.size() + that.M_f.size() <= 64) {
      M_naive_multiplication(that);
      return *this;
    }

    size_type n = ceil2(M_f.size() + that.M_f.size() - 1);

    if (ctz(n) > ctz(value_type::get_modulo()-1)) {
      M_arbitrary_modulo_convolve(std::move(that));
      return *this;
    }

    M_f.resize(n, 0);
    that.M_f.resize(n, 0);
    M_fft();
    that.M_fft();
    for (size_type i = 0; i < n; ++i)
      M_f[i] *= that.M_f[i];
    M_ifft();
    M_normalize();
    return *this;
  }

  polynomial& operator /=(polynomial that) {
    if (M_f.size() < that.M_f.size()) {
      M_f[0] = 0;
      M_f.erase(M_f.begin()+1, M_f.end());
      return *this;
    }

    if (that.M_f.size() == 1) {
      // scalar division
      value_type d = 1 / that.M_f[0];
      for (auto& c: M_f) c *= d;
      return *this;
    }
    if (that.M_f.size() <= 256) {
      M_naive_division(that);
      return *this;
    }

    size_type deg = M_f.size() - that.M_f.size() + 1;
    std::reverse(M_f.begin(), M_f.end());
    std::reverse(that.M_f.begin(), that.M_f.end());
    *this *= that.inverse(deg);
    M_f.resize(deg);
    std::reverse(M_f.begin(), M_f.end());
    M_normalize();
    return *this;
  }

  polynomial& operator %=(polynomial that) {
    if (M_f.size() < that.M_f.size()) return *this;
    *this -= *this / that * that;
    return *this;
  }

  polynomial operator +(polynomial const& that) const {
    return polynomial(*this) += that;
  }
  polynomial operator -(polynomial const& that) const {
    return polynomial(*this) -= that;
  }
  polynomial operator *(polynomial const& that) const {
    return polynomial(*this) *= that;
  }
  polynomial operator /(polynomial const& that) const {
    return polynomial(*this) /= that;
  }
  polynomial operator %(polynomial const& that) const {
    return polynomial(*this) %= that;
  }

  value_type const operator [](size_type i) const {
    return ((i < M_f.size())? M_f[i]: 0);
  }

  value_type operator ()(value_type x) const {
    value_type y = 0;
    for (size_type i = M_f.size(); i--;) y = y * x + M_f[i];
    return y;
  }

  bool zero() const noexcept { return M_f.empty(); }
  size_type degree() const { return M_f.size()-1; }  // XXX deg(0)

  void fft(size_type n = 0) { if (n) M_f.resize(n, value_type{0}); M_fft(); }
  void ifft(size_type n = 0) { if (n) M_f.resize(n, value_type{0}); M_ifft(); }
};


#line 1 "integer/sqrt.cpp"



/**
 * @brief 整数の平方根
 * @author えびちゃん
 */

#line 10 "integer/sqrt.cpp"

template <typename Tp>
Tp isqrt(Tp n) {
  if (n <= 1) return n;
  Tp lb = 1;
  Tp ub = static_cast<Tp>(1) << (CHAR_BIT * (sizeof(Tp) / 2));
  while (ub-lb > 1) {
    Tp mid = (lb+ub) >> 1;
    ((mid*mid <= n)? lb: ub) = mid;
  }
  return lb;
}


#line 17 "ModularArithmetic/factorial.cpp"

template <intmax_t Mod>
modint<Mod> factorial(intmax_t n) {
  if (n == 0) return 1;
  if (n >= Mod) return 0;
  intmax_t v = ceil2(isqrt(n));
  using value_type = modint<Mod>;
  modtable<value_type> mt(v);

  std::vector<value_type> g{1, v+1};
  for (intmax_t d = 1; d < v; d <<= 1) {
    std::vector<value_type> a(d+1);
    for (intmax_t i = 0; i <= d; ++i) {
      a[i] = g[i] * mt.factorial_inverse(i) * mt.factorial_inverse(d-i);
      if ((d-i) % 2 != 0) a[i] = -a[i];
    }

    auto shift = [&](intmax_t m0) {
      m0 = (value_type(m0) / v).get();
      intmax_t m = std::max(m0, d+1);

      std::vector<value_type> b(d+1), c(d+1);
      for (intmax_t i = 0; i <= d; ++i) {
        b[i] = value_type{1} / (m+i);
        c[d-i] = value_type{1} / (m-i-1);
      }

      polynomial<value_type> fa(a.begin(), a.end()), fb(b.begin(), b.end()), fc(c.begin(), c.end());
      fb *= fa, fc *= fa;

      std::vector<value_type> res(d+1);
      value_type mul = 1;
      for (int j = 0; j <= d; ++j) mul *= m-j;
      for (int k = 0; k <= d; ++k) {
        res[k] = (fb[k] + fc[k+d+1]) * mul;
        if (k < d) mul *= value_type(m+k+1) / value_type(m+k-d);
      }
      if (m0 <= d) {
        for (intmax_t i = m0; i--;) res[d-(m0-i-1)] = res[i];
        for (intmax_t i = m0; i <= d; ++i) res[i-m0] = g[i];
      }
      return res;
    };
    std::vector<value_type> gd = shift(d);
    std::vector<value_type> gdv = shift(d*v);
    std::vector<value_type> gdvd = shift(d*v+d);

    g.resize(2*d+1);
    for (intmax_t i = 0; i < d; ++i) g[i] *= gd[i];
    for (intmax_t i = 0; i <= d; ++i) g[d+i] = gdv[i] * gdvd[i];
  }

  value_type res = 1;
  intmax_t i = 0;
  for (size_t j = 0; i+v <= n && j < g.size(); i += v, ++j) res *= g[j];
  for (++i; i <= n; ++i) res *= i;
  return res;
}



```
{% endraw %}

<a href="../../index.html">Back to top page</a>
