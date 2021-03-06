#ifndef H_suffix_array
#define H_suffix_array

/**
 * @brief 接尾辞配列
 * @author えびちゃん
 */

#include <cstddef>
#include <algorithm>
#include <numeric>
#include <vector>

#include "utility/literals.cpp"

template <typename Tp>
class suffix_array {
public:
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using value_type = Tp;

private:
  std::vector<value_type> M_c;
  size_type M_len;  // including trailing '$'
  std::vector<size_type> M_rank, M_sa, M_lcpa;

  void M_build_sa() {
    M_sa.resize(M_len);
    std::iota(M_sa.begin(), M_sa.end(), 0_zu);

    {
      auto comp = [&](size_type i, size_type j) -> bool {
        if (i < M_c.size()) {
          if (j >= M_c.size()) return false;
          if (j < M_c.size()) {
            if (M_c[i] < M_c[j]) return true;
            if (M_c[j] < M_c[i]) return false;
          }
        } else if (j < M_c.size()) {
          return true;
        }
        size_type k = 1;
        if (i+k < M_c.size()) {
          if (j+k < M_c.size()) return M_c[i+k] < M_c[j+k];
          return false;
        }
        return j+k < M_c.size();
      };
      std::sort(M_sa.begin(), M_sa.end(), comp);
      std::vector<size_type> tmp(M_len);
      tmp[M_sa[0]] = 0;
      for (size_type i = 1; i < M_len; ++i) {
        tmp[M_sa[i]] = tmp[M_sa[i-1]];
        if (comp(M_sa[i-1], M_sa[i])) ++tmp[M_sa[i]];
      }
      M_rank = std::move(tmp);
    }

    for (size_type k = 2; k < M_len; k <<= 1) {
      auto comp = [&](size_type i, size_type j) -> bool {
        if (M_rank[i] < M_rank[j] || M_rank[j] < M_rank[i])
          return M_rank[i] < M_rank[j];

        size_type ri = ((i+k < M_len)? M_rank[i+k]+1: 0_zu);
        size_type rj = ((j+k < M_len)? M_rank[j+k]+1: 0_zu);
        return ri < rj;
      };
      std::sort(M_sa.begin(), M_sa.end(), comp);
      std::vector<size_type> tmp(M_len);
      tmp[M_sa[0]] = 0;
      for (size_type i = 1; i < M_len; ++i) {
        tmp[M_sa[i]] = tmp[M_sa[i-1]];
        if (comp(M_sa[i-1], M_sa[i])) ++tmp[M_sa[i]];
      }
      M_rank = std::move(tmp);
    }
  }

  void M_build_lcpa() {
    size_type n = M_c.size();
    std::vector<size_type> rank(n+1);
    for (size_type i = 0; i <= n; ++i) rank[M_sa[i]] = i;
    size_type h = 0;
    M_lcpa.assign(n+1, 0);
    for (size_type i = 0; i < n; ++i) {
      size_type j = M_sa[rank[i]-1];
      if (h > 0) --h;
      for (; (j+h < n && i+h < n); ++h) {
        if (M_c[j+h] < M_c[i+h] || M_c[i+h] < M_c[j+h]) break;
      }
      M_lcpa[rank[i]-1] = h;
    }
  }

  template <typename InputIt>
  bool M_lexicographical_compare(size_type pos, InputIt first, InputIt last) const {
    // return true if M_c[pos:] < *[first, last)
    while (first != last) {
      if (pos == M_c.size()) return true;
      if (M_c[pos] < *first) return true;
      if (*first < M_c[pos]) return false;
      ++pos, ++first;
    }
    return false;
  }

  template <typename InputIt>
  InputIt M_mismatch(size_type pos, InputIt first, InputIt last) const {
    // with equivalence, instead of equality
    while (first != last) {
      if (pos == M_c.size()) break;
      if (M_c[pos] < *first || *first < M_c[pos]) break;
      ++pos, ++first;
    }
    return first;
  }

public:
  suffix_array() = default;
  suffix_array(suffix_array const&) = default;
  suffix_array(suffix_array&&) = default;

  template <typename InputIt>
  suffix_array(InputIt first, InputIt last):
    M_c(first, last), M_len(M_c.size()+1)
  { M_build_sa(), M_build_lcpa(); }

  template <typename ForwardIt>
  difference_type lcp(ForwardIt first, ForwardIt last) const {
    size_type lb = 0;
    size_type ub = M_c.size();
    while (ub-lb > 1) {
      size_type mid = (lb+ub) >> 1;
      (M_lexicographical_compare(M_sa[mid], first, last)? lb: ub) = mid;
    }
    auto it0 = M_mismatch(M_sa[lb], first, last);
    auto it1 = M_mismatch(M_sa[ub], first, last);
    return std::max(std::distance(first, it0), std::distance(first, it1));
  }

  template <typename ForwardIt>
  size_type lower_bound(ForwardIt first, ForwardIt last) const {
    size_type lb = 0;
    size_type ub = M_c.size();
    while (ub-lb > 1) {
      size_type mid = (lb+ub) >> 1;
      (M_lexicographical_compare(M_sa[mid], first, last)? lb: ub) = mid;
    }
    if (M_lexicographical_compare(M_sa[ub], first, last)) return M_c.size();
    return M_sa[ub];
  }

  template <typename ForwardIt>
  bool contains(ForwardIt first, ForwardIt last) const {
    return lcp(first, last) == std::distance(first, last);
  }

  size_type operator [](size_type i) const { return M_sa[i]; }
  std::vector<size_type> const& get_lcpa() const { return M_lcpa; }
};

#endif  /* !defined(H_suffix_array) */
