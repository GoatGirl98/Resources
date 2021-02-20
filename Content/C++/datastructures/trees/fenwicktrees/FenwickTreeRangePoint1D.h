#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and point queries in 1 dimension
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Order of arguments for update differs slightly from FenwickTreeRangePoint
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   values(): returns a vector of the fenwick tree decomposed into an array
//   update(l, v): add v to the range [l..N - 1]
//   update(l, r, v): add v to the range [l..r]
//   get(i): queries the value at index i
//   bsearch(v, cmp): returns the first index where cmp(A[i], v)
//     returns false, or N if no such index exists
//   lower_bound(v): returns the first index where A[i] >= v, assumes
//     A is sorted by cmp
//   upper_bound(v): returns the first index where A[i] > v, assumes
//     A is sorted by cmp
// In practice, this version performs as well as the multidimensional version
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor, values: O(N)
//   update, get: O(log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://mcpt.ca/problem/asquirrelproblem
//   https://codeforces.com/contest/1404/problem/C
template <class T> struct FenwickTreeRangePoint1D {
  int N; vector<T> BIT;
  FenwickTreeRangePoint1D(int N) : N(N), BIT(N + 1, T()) {}
  template <class F> FenwickTreeRangePoint1D(int N, F f)
      : FenwickTreeRangePoint1D(N) {
    T prv = T(); for (int i = 1; i <= N; i++) {
      T cur = f(); BIT[i] += cur - prv; prv = cur;
      int j = i + (i & -i); if (j <= N) BIT[j] += BIT[i];
    }
  }
  template <class It> FenwickTreeRangePoint1D(It st, It en)
      : FenwickTreeRangePoint1D(en - st, [&] { return *st++; }) {}
  vector<T> values() {
    vector<T> ret(BIT.begin() + 1, BIT.end()); for (int i = N; i >= 1; i--) {
      int j = i + (i & -i); if (j <= N) ret[j - 1] -= ret[i - 1];
    }
    partial_sum(ret.begin(), ret.end(), ret.begin()); return ret;
  }
  void update(int l, T v) { for (l++; l <= N; l += l & -l) BIT[l] += v; }
  void update(int l, int r, T v) { update(l, v); update(r + 1, -v); }
  T get(int i) {
    T ret = T(); for (i++; i > 0; i -= i & -i) ret += BIT[i];
    return ret;
  }
  template <class F> int bsearch(T v, F cmp) {
    T val = T(); int ind = 0; for (int j = __lg(N + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i <= N && cmp(val + BIT[i], v)) val += BIT[ind = i];
    }
    return ind;
  }
  int lower_bound(T v) { return bsearch(v, less<T>()); }
  int upper_bound(T v) { return bsearch(v, less_equal<T>()); }
};
