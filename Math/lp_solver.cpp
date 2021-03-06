#ifndef H_lp_solver
#define H_lp_solver

/**
 * @brief 線形計画法ソルバ (simplex 法)
 * @author えびちゃん
 */

#include <cstddef>
#include <cmath>
#include <numeric>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "utility/limits.cpp"
#include "utility/literals.cpp"

template <typename Tp>
class lp_solver {
public:
  using size_type = size_t;
  using value_type = Tp;

private:
  using vector_type = std::vector<value_type>;
  using matrix_type = std::vector<vector_type>;
  using indices_type = std::vector<size_type>;
  constexpr static value_type const S_unbounded = limits<value_type>::infinity();

  static bool S_is_nonnegative(value_type v) {
    if constexpr (!std::is_floating_point<value_type>::value) return v >= 0;
    return v > -1e-9;
  }
  static bool S_is_nonzero(value_type v) {
    if constexpr (!std::is_floating_point<value_type>::value) return v != 0;
    else return std::abs(v) > value_type{1e-9};
  }
  static bool S_is_unbounded(value_type v) { return v == S_unbounded; }
  static bool S_is_equal_to(value_type v, value_type w) { return !S_is_nonzero(v-w); }

  static size_type S_any_positive(vector_type const& c) {
    for (size_type i = 0; i < c.size(); ++i)
      if (c[i] > 0) return i;
    return -1_zu;
  }

  static size_type S_argmin(vector_type const& v, indices_type const& is) {
    size_type argmin = 0;
    value_type min = v[0];
    for (size_type i = 1; i < v.size(); ++i) {
      if (v[i] < min || (S_is_equal_to(v[i], min) && is[i] < is[argmin])) {
        argmin = i;
        min = v[i];
      }
    }
    return argmin;
  }

  static void S_pivot(
      indices_type& ni, indices_type& bi, matrix_type& a,
      vector_type& b, vector_type& c, value_type& v, size_type l, size_type e
  ) {
    size_type m = b.size(), n = c.size();
    b[l] /= a[l][e];
    a[l][e] = value_type{1} / a[l][e];
    for (size_type j = 0; j < n; ++j) {
      if (j != e) a[l][j] *= a[l][e];
    }
    for (size_type i = 0; i < m; ++i) {
      if (i == l) continue;
      b[i] -= a[i][e] * b[l];
      for (size_type j = 0; j < n; ++j) {
        if (j != e) a[i][j] -= a[i][e] * a[l][j];
      }
      a[i][e] *= -a[l][e];
    }
    v += c[e] * b[l];

    for (size_type j = 0; j < n; ++j) {
      if (j != e) c[j] -= c[e] * a[l][j];
    }
    c[e] *= -a[l][e];
    std::swap(ni[e], bi[l]);
  }

  static bool S_initialize(
      indices_type& ni, indices_type& bi, matrix_type& a,
      vector_type& b, vector_type& c, value_type& v
  ) {
    size_type m = b.size(), n = c.size();
    ni.resize(n);
    bi.resize(m);
    std::iota(ni.begin(), ni.end(), 0_zu);
    std::iota(bi.begin(), bi.end(), n);
    v = value_type{0};
    size_type k = S_argmin(b, bi);
    if (S_is_nonnegative(b[k])) return true;

    for (auto& ai: a) ai.emplace_back(-1_zu);
    ni.push_back(n+m);

    vector_type c0(n+1);
    c0[n] = value_type{-1};
    S_pivot(ni, bi, a, b, c0, v, k, n);
    S_simplex(ni, bi, a, b, c0, v);

    for (size_type i = 0; i < bi.size(); ++i) {
      if (bi[i] != n+m) continue;
      if (S_is_nonzero(b[bi[i]])) return false;
      S_pivot(ni, bi, a, b, c0, v, i, n);
      break;
    }

    for (size_type j = 0; j <= n; ++j) {
      if (ni[j] != n+m) continue;
      for (auto& ai: a) ai.erase(ai.begin()+j);
      c0.erase(c0.begin()+j);
      ni.erase(ni.begin()+j);
      break;
    }

    vector_type c1(n);
    for (size_type j = 0; j < n; ++j) {
      if (size_type n0 = ni[j]; n0 < n) c1[j] = c[n0];
    }
    for (size_type i = 0; i < m; ++i) {
      size_type b0 = bi[i];
      if (b0 >= n) continue;
      for (size_type j = 0; j < n; ++j) {
        c1[j] -= c[b0] * a[i][j];
      }
      v += c[b0] * b[i];
    }
    c = std::move(c1);
    return true;
  }

  static bool S_simplex(
      indices_type& ni, indices_type& bi, matrix_type& a,
      vector_type& b, vector_type& c, value_type& v
  ) {
    size_type m = b.size();
    while (true) {
      size_type e = S_any_positive(c);
      if (e == -1_zu) return true;
      vector_type delta(m, S_unbounded);
      for (size_type i = 0; i < m; ++i) {
        if (a[i][e] > 0) delta[i] = b[i] / a[i][e];
      }
      size_type l = S_argmin(delta, bi);
      if (S_is_unbounded(delta[l])) return false;
      S_pivot(ni, bi, a, b, c, v, l, e);
    }
  }

public:
  lp_solver() = default;

  static std::optional<value_type> maximize(
      matrix_type& a, vector_type& b, vector_type& c
  ) {
    indices_type ni, bi;
    value_type v{0};
    if (!S_initialize(ni, bi, a, b, c, v)) return {};
    if (!S_simplex(ni, bi, a, b, c, v)) return S_unbounded;
    return v;
  }
};

template <typename Tp>
constexpr Tp const lp_solver<Tp>::S_unbounded;

#endif  /* !defined(H_lp_solver) */
