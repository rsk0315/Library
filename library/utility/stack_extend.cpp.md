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


# :heavy_check_mark: スタック拡張マクロ（魔法） <small>(utility/stack_extend.cpp)</small>

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#67b732dc42aaffa9056d34cc477c863c">utility</a>
* <a href="{{ site.github.repository_url }}/blob/master/utility/stack_extend.cpp">View this file on GitHub</a>
    - Last commit date: 2020-03-17 11:00:02+09:00


* see: <a href="http://sigma425.hatenablog.com/entry/2016/03/26/221844">http://sigma425.hatenablog.com/entry/2016/03/26/221844</a>


## Verified with

* :heavy_check_mark: <a href="../../verify/test/aoj_GRL_5_C_segment_tree.test.cpp.html">test/aoj_GRL_5_C_segment_tree.test.cpp</a>
* :heavy_check_mark: <a href="../../verify/test/aoj_GRL_5_C_sparse_table.test.cpp.html">test/aoj_GRL_5_C_sparse_table.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
/**
 * @brief スタック拡張マクロ（魔法）
 * @author えびちゃん
 * @see http://sigma425.hatenablog.com/entry/2016/03/26/221844
 */

#ifndef H_stack_extend
#define H_stack_extend

#include <cstdlib>

#define BEGIN_STACK_EXTEND(size)                                        \
  void* stack_extend_memory_ = malloc(size);                            \
  void* stack_extend_origin_memory_;                                    \
  char* stack_extend_dummy_memory_ = (char*)alloca((1+(int)(((long long)stack_extend_memory_)&127))*16); \
  *stack_extend_dummy_memory_ = 0;                                      \
  asm volatile ("mov %%rsp, %%rbx\n\tmov %%rax, %%rsp":"=b"(stack_extend_origin_memory_):"a"((char*)stack_extend_memory_+(size)-1024));

#define END_STACK_EXTEND                                                \
  asm volatile ("mov %%rax, %%rsp"::"a"(stack_extend_origin_memory_));  \
  free(stack_extend_memory_);

#endif  /* !defined(H_stack_extend) */

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "utility/stack_extend.cpp"
/**
 * @brief スタック拡張マクロ（魔法）
 * @author えびちゃん
 * @see http://sigma425.hatenablog.com/entry/2016/03/26/221844
 */

#ifndef H_stack_extend
#define H_stack_extend

#include <cstdlib>

#define BEGIN_STACK_EXTEND(size)                                        \
  void* stack_extend_memory_ = malloc(size);                            \
  void* stack_extend_origin_memory_;                                    \
  char* stack_extend_dummy_memory_ = (char*)alloca((1+(int)(((long long)stack_extend_memory_)&127))*16); \
  *stack_extend_dummy_memory_ = 0;                                      \
  asm volatile ("mov %%rsp, %%rbx\n\tmov %%rax, %%rsp":"=b"(stack_extend_origin_memory_):"a"((char*)stack_extend_memory_+(size)-1024));

#define END_STACK_EXTEND                                                \
  asm volatile ("mov %%rax, %%rsp"::"a"(stack_extend_origin_memory_));  \
  free(stack_extend_memory_);

#endif  /* !defined(H_stack_extend) */

```
{% endraw %}

<a href="../../index.html">Back to top page</a>
