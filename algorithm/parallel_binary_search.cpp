#ifndef H_parallel_binary_search
#define H_parallel_binary_search

/**
 * @brief 並列二分探索
 * @author えびちゃん
 */

#include <cstddef>
#include <vector>

template <typename StatefulPredicate, typename RandomIt>
std::vector<size_t> parallel_binary_search(
    StatefulPredicate spred, RandomIt first, RandomIt last
) {
  // result[i] = j if spred(first[i]) returns true for state j-1 (and
  // before that) and returns false for state j (and after that).
  size_t size = spred.size();
  std::vector<size_t> lb(std::distance(first, last), 0);
  std::vector<size_t> ub(lb.size(), size+1);
  bool determined = false;
  while (!determined) {
    determined = true;
    std::vector<std::vector<RandomIt>> ev(size+1);
    auto it = first;
    for (size_t i = 0; i < lb.size(); ++i) {
      auto cur = it++;
      if (!(ub[i] - lb[i] > 1)) continue;
      size_t mid = (lb[i] + ub[i]) >> 1;
      ev[mid].push_back(cur);
      determined = false;
    }
    auto sp = spred;
    for (size_t i = 0; i <= size; ++i) {
      for (auto const& e: ev[i]) {
        (sp(*e)? lb[e-first]: ub[e-first]) = i;
      }
      if (i < size) sp.next();
    }
  }
  return ub;
}

#endif  /* !defined(H_parallel_binary_search) */
