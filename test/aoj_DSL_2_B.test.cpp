#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B"

#include <cstdint>
#include <cstdio>
#include <vector>

#include "DataStructure/basic_segment_tree.cpp"

int main() {
  size_t n, q;
  scanf("%zu %zu", &n, &q);

  basic_segment_tree<intmax_t> st(n, 0);
  for (size_t i = 0; i < q; ++i) {
    int com;
    scanf("%d", &com);

    if (com == 0) {
      size_t i;
      intmax_t x;
      scanf("%zu %jd", &i, &x);
      --i;
      st.set(i, st[i]+x);
    } else if (com == 1) {
      size_t x, y;
      scanf("%zu %zu", &x, &y);
      --x, --y;
      printf("%jd\n", st.fold(x, y+1));
    }
  }
}
