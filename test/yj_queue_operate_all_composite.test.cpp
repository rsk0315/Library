#define PROBLEM "https://judge.yosupo.jp/problem/queue_operate_all_composite"

#include <cstdint>
#include <cstdio>

#include "utility/monoid/composite.cpp"
#include "ModularArithmetic/modint.cpp"
#include "DataStructure/foldable_queue.cpp"

constexpr intmax_t mod = 998244353;
using mi = modint<mod>;

int main() {
  size_t q;
  scanf("%zu", &q);

  foldable_queue<composite_monoid<mi>> fq;
  for (size_t i = 0; i < q; ++i) {
    int t;
    scanf("%d", &t);

    if (t == 0) {
      intmax_t a, b;
      scanf("%jd %jd", &a, &b);
      fq.emplace(a, b);
    } else if (t == 1) {
      fq.pop();
    } else if (t == 2) {
      intmax_t x;
      scanf("%jd", &x);
      printf("%d\n", fq.fold()(x).get());
    }
  }
}
