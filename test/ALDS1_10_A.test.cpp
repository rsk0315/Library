#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_A"

#define CALL_FROM_TEST
#include "utility/make/fix_point.cpp"
#undef CALL_FROM_TEST

#include <cstdint>
#include <cstdio>
#include <vector>

int main() {
  int n;
  scanf("%d", &n);

  std::vector<intmax_t> memo(n+1);
  memo[0] = memo[1] = 1;
  make_fix_point([&](auto f, int i) {
      intmax_t& res = memo[i];
      if (res > 0) return res;
      res = f(i-1) + f(i-2);
      return res;
  })(n);
  printf("%jd\n", memo[n]);
}
