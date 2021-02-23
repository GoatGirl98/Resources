#pragma once
#include <bits/stdc++.h>
using namespace std;

// Knuth's Dynamic Programming Optimization
// Must satisfy dp[l][r] = max(dp[l][m] + dp[m][r] + cost(l, m, r))
//   for l <= m <= r and
//   max(l + 1, opt[l][r - 1]) <= opt[l][r] <= min(opt[l + 1][r], r - 1),
//   where opt[l][r] is the optimal value of m for dp[l][r]
// Template Arguments:
//   T: the type of the return value of the cost function
//   F: the type of the function used to compute the cost function
//   Cmp: the comparator to compare two T values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   N: the size of the array, must be positive
//   f(l, m, r): computes the cost function for the range [l, r) with the
//     midpoint m
//   cmp: an instance of the Cmp struct
// Return Value: a dp array of size N x (N + 1) with dp[l][r] equal to the
//   maximum dp value (based on the comparator) of the range [l, r)
// In practice, has a small constant
// Time Complexity: O(N^2)
// Memory Complexity: O(N^2)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_n
template <class T, class F, class Cmp = less<T>>
vector<vector<T>> knuth(int N, F f, Cmp cmp = Cmp()) {
  assert(N > 0); vector<vector<T>> dp(N, vector<T>(N + 1, T()));
  vector<vector<int>> opt(N, vector<int>(N + 1));
  for (int l = N - 1; l >= 0; l--) for (int r = l; r <= N; r++) {
    if (r - l <= 1) { opt[l][r] = l; continue; }
    int st = max(l + 1, opt[l][r - 1]), en = min(opt[l + 1][r], r - 1);
    for (int m = st; m <= en; m++) {
      T cost = dp[l][m] + dp[m][r] + f(l, m, r);
      if (m == st || cmp(dp[l][r], cost)) { dp[l][r] = cost; opt[l][r] = m; }
    }
  }
  return dp;
}
