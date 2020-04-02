#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#define CALL_FROM_TEST
#include "DataStructure/wavelet_matrix.cpp"
#undef CALL_FROM_TEST

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

int main() {
  size_t n, q;
  scanf("%zu %zu", &n, &q);

  std::vector<uintmax_t> a(n);
  for (auto& ai: a) scanf("%ju", &ai);

  wavelet_matrix<32> wm(a.begin(), a.end());
  for (size_t i = 0; i < q; ++i) {
    size_t l, r, k;
    scanf("%zu %zu %zu", &l, &r, &k);
    printf("%ju\n", wm.quantile(k, l, r));
  }
}
