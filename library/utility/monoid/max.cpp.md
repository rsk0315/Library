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
<script type="text/javascript" src="../../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../../assets/css/copy-button.css" />


# :warning: max を得る演算のモノイド <small>(utility/monoid/max.cpp)</small>

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#0991b1681f77f54af5325f2eb1ef5d3e">utility/monoid</a>
* <a href="{{ site.github.repository_url }}/blob/master/utility/monoid/max.cpp">View this file on GitHub</a>
    - Last commit date: 2020-03-22 02:59:14+09:00




## Code

<a id="unbundled"></a>
{% raw %}
```cpp
/**
 * @brief max を得る演算のモノイド
 * @author えびちゃん
 */

#ifdef CALL_FROM_TEST
#include "utility/limits.cpp"
#endif

#include <algorithm>
#include <utility>

#ifndef H_max_monoid
#define H_max_monoid

template <typename Tp>
class max_monoid {
public:
  using value_type = Tp;

private:
  value_type M_x = limits<value_type>::min();

public:
  max_monoid() = default;  // identity
  max_monoid(max_monoid const&) = default;
  max_monoid(max_monoid&&) = default;

  max_monoid(value_type const& x): M_x(x) {};
  max_monoid(value_type&& x): M_x(std::move(x)) {};

  max_monoid& operator =(max_monoid const&) = default;
  max_monoid& operator =(max_monoid&&) = default;

  max_monoid& operator +=(max_monoid const& that) {
    M_x = std::max(M_x, that.M_x);
    return *this;
  }
  max_monoid& operator +=(max_monoid&& that) {
    M_x = std::max(M_x, std::move(that.M_x));
    return *this;
  }

  max_monoid operator +(max_monoid const& that) const {
    return max_monoid(*this) += that;
  }
  max_monoid operator +(max_monoid&& that) const {
    return max_monoid(*this) += std::move(that);
  }

  value_type const& get() const { return M_x; }
};

#endif  /* !defined(H_max_monoid) */

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
Traceback (most recent call last):
  File "/opt/hostedtoolcache/Python/3.8.2/x64/lib/python3.8/site-packages/onlinejudge_verify/docs.py", line 340, in write_contents
    bundled_code = language.bundle(self.file_class.file_path, basedir=pathlib.Path.cwd())
  File "/opt/hostedtoolcache/Python/3.8.2/x64/lib/python3.8/site-packages/onlinejudge_verify/languages/cplusplus.py", line 170, in bundle
    bundler.update(path)
  File "/opt/hostedtoolcache/Python/3.8.2/x64/lib/python3.8/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py", line 281, in update
    raise BundleError(path, i + 1, "unable to process #include in #if / #ifdef / #ifndef other than include guards")
onlinejudge_verify.languages.cplusplus_bundle.BundleError: utility/monoid/max.cpp: line 7: unable to process #include in #if / #ifdef / #ifndef other than include guards

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
