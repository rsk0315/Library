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


# :heavy_check_mark: test/yj_scc.test.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#098f6bcd4621d373cade4e832627b4f6">test</a>
* <a href="{{ site.github.repository_url }}/blob/master/test/yj_scc.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-04-06 05:07:20+09:00


* see: <a href="https://judge.yosupo.jp/problem/scc">https://judge.yosupo.jp/problem/scc</a>


## Depends on

* :heavy_check_mark: <a href="../../library/Graph/adjacency_list.cpp.html">重みつきグラフの隣接リスト <small>(Graph/adjacency_list.cpp)</small></a>
* :heavy_check_mark: <a href="../../library/Graph/scc.cpp.html">強連結成分分解 <small>(Graph/scc.cpp)</small></a>
* :heavy_check_mark: <a href="../../library/utility/make/fix_point.cpp.html">ラムダ式の再帰 <small>(utility/make/fix_point.cpp)</small></a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include <cstdio>
#include <algorithm>
#include <vector>

#include "Graph/adjacency_list.cpp"
#include "Graph/scc.cpp"

int main() {
  size_t n, m;
  scanf("%zu %zu", &n, &m);

  adjacency_list<weighted_edge<int>, directed_tag> g(n);
  for (size_t i = 0; i < m; ++i) {
    size_t u, v;
    scanf("%zu %zu", &u, &v);
    g.emplace(u, v, 1);
  }

  auto scc = strongly_connected_components(g);
  size_t k = *std::max_element(scc.begin(), scc.end()) + 1;

  std::vector<std::vector<size_t>> ts(k);
  for (size_t i = 0; i < n; ++i)
    ts[scc[i]].push_back(i);

  printf("%zu\n", k);
  for (size_t i = 0; i < k; ++i) {
    printf("%zu", ts[i].size());
    for (auto v: ts[i]) printf(" %zu", v);
    puts("");
  }
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "test/yj_scc.test.cpp"
#define PROBLEM "https://judge.yosupo.jp/problem/scc"

#include <cstdio>
#include <algorithm>
#include <vector>

#line 1 "Graph/adjacency_list.cpp"



/**
 * @brief 重みつきグラフの隣接リスト
 * @author えびちゃん
 */

#include <cstddef>
#line 11 "Graph/adjacency_list.cpp"
#include <type_traits>
#line 13 "Graph/adjacency_list.cpp"

template <typename WeightType>
class weighted_edge {
public:
  using size_type = size_t;
  using weight_type = WeightType;

protected:
  size_type M_src, M_dst;
  weight_type M_weight;

public:
  weighted_edge() = default;
  weighted_edge(weighted_edge const&) = default;
  weighted_edge(weighted_edge&&) = default;

  weighted_edge(size_type s, size_type d, weight_type w):
    M_src(s), M_dst(d), M_weight(w)
  {}

  weighted_edge& operator =(weighted_edge const&) = default;
  weighted_edge& operator =(weighted_edge&&) = default;

  bool operator <(weighted_edge const& other) const {
    if (M_weight < other.M_weight) return true;
    if (other.M_weight < M_weight) return false;
    if (M_src != other.M_src) return M_src < other.M_src;
    return M_dst < other.M_dst;
  }

  size_type source() const { return M_src; }
  size_type target() const { return M_dst; }
  weight_type weight() const { return M_weight; }
};

struct directed_tag {};
struct undirected_tag {};

template <typename Edge, typename Directedness>
class adjacency_list {
public:
  using size_type = size_t;
  using edge_type = Edge;
  using weight_type = typename Edge::weight_type;

private:
  std::vector<std::vector<edge_type>> M_g;

public:
  adjacency_list() = default;
  adjacency_list(adjacency_list const&) = default;
  adjacency_list(adjacency_list&&) = default;
  explicit adjacency_list(size_type n): M_g(n) {}

  template <typename... Args>
  void emplace(size_type src, size_type dst, Args... args) {
    M_g[src].emplace_back(src, dst, args...);
    if (std::is_same<Directedness, undirected_tag>::value)
      M_g[dst].emplace_back(dst, src, args...);
  }

  void sort_by_index() {
    auto cmp = [](auto const& e1, auto const& e2) {
      return e1.target() < e2.target();
    };
    for (auto v: M_g) std::sort(v.begin(), v.end(), cmp);
  }

  size_type size() const { return M_g.size(); }
  auto const& operator [](size_type i) const { return M_g[i]; }
};


#line 1 "Graph/scc.cpp"



/**
 * @brief 強連結成分分解
 * @author えびちゃん
 */

#line 11 "Graph/scc.cpp"

#line 1 "utility/make/fix_point.cpp"
/**
 * @brief ラムダ式の再帰
 * @author えびちゃん
 */

#ifndef H_make_fix_point
#define H_make_fix_point

#include <utility>

template <typename Fn>
class fix_point: Fn {
public:
  explicit constexpr fix_point(Fn&& f) noexcept: Fn(std::forward<Fn>(f)) {}

  template <typename... Args>
  constexpr decltype(auto) operator ()(Args&&... args) const {
    return Fn::operator ()(*this, std::forward<Args>(args)...);
  }
};

template <typename Fn>
static inline constexpr decltype(auto) make_fix_point(Fn&& f) noexcept {
  return fix_point<Fn>{std::forward<Fn>(f)};
}

#endif  /* !defined(H_make_fix_point) */
#line 14 "Graph/scc.cpp"

template <typename AdjacencyList>
auto strongly_connected_components(AdjacencyList const& g) {
  size_t n = g.size();
  adjacency_list<weighted_edge<bool>, directed_tag> h(n);
  for (size_t v = 0; v < n; ++v)
    for (auto const& e: g[v]) h.emplace(e.target(), e.source(), 1);

  std::vector<bool> used(n);
  std::vector<size_t> vs;
  auto dfs = make_fix_point([&](auto f, size_t v) -> void {
      used[v] = true;
      for (auto const& e: g[v])
        if (!used[e.target()]) f(e.target());
      vs.push_back(v);
  });
  for (size_t v = 0; v < n; ++v)
    if (!used[v]) dfs(v);

  used.assign(n, false);
  std::vector<size_t> cmp(n);
  size_t num = 0;
  auto rdfs = make_fix_point([&](auto f, size_t v) -> void {
      used[v] = true;
      cmp[v] = num;
      for (auto const& e: h[v])
        if (!used[e.target()]) f(e.target());
  });
  for (size_t v = vs.size(); v--;)
    if (!used[vs[v]])
      rdfs(vs[v]), ++num;

  return cmp;
}


#line 9 "test/yj_scc.test.cpp"

int main() {
  size_t n, m;
  scanf("%zu %zu", &n, &m);

  adjacency_list<weighted_edge<int>, directed_tag> g(n);
  for (size_t i = 0; i < m; ++i) {
    size_t u, v;
    scanf("%zu %zu", &u, &v);
    g.emplace(u, v, 1);
  }

  auto scc = strongly_connected_components(g);
  size_t k = *std::max_element(scc.begin(), scc.end()) + 1;

  std::vector<std::vector<size_t>> ts(k);
  for (size_t i = 0; i < n; ++i)
    ts[scc[i]].push_back(i);

  printf("%zu\n", k);
  for (size_t i = 0; i < k; ++i) {
    printf("%zu", ts[i].size());
    for (auto v: ts[i]) printf(" %zu", v);
    puts("");
  }
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
