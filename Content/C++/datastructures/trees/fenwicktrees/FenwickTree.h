#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range sum queries in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   D: the number of dimensions of the fenwick tree
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the first dimension
//   ...args: the sizes of the second, third, fourth, etc. dimensions
// Functions:
//   update(i, ...args): updates the index i in the first dimension,
//     and ...args for the next dimensions, with the last argument
//     being the value to add to the index
//   query(l, r, ...args): queries the sum of the range [l, r] in the first
//     dimension, and alternating arguments in args for beginning and ending
//     ranges for the next dimensions
// In practice, has a small constant, faster than segment trees and comparable
//   to the 1D version
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(PI(log(N_i)))
//   query: O(2^D * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/ds1
//   https://judge.yosupo.jp/problem/point_add_range_sum
//   https://dmoj.ca/problem/ioi01p1
//   https://dmoj.ca/problem/gfssoc1s4
//   https://dmoj.ca/problem/fallingsnowflakes
template <const int D, class T> struct FenwickTree {
  int N; vector<FenwickTree<D - 1, T>> BIT;
  template <class ...Args> FenwickTree(int N, Args &&...args)
      : N(N), BIT(N + 1, FenwickTree<D - 1, T>(forward<Args>(args)...)) {}
  template <class ...Args> void update(int i, Args &&...args) {
    for (i++; i <= N; i += i & -i) BIT[i].update(forward<Args>(args)...);
  }
  template <class ...Args> T query(int l, int r, Args &&...args) {
    T ret = T();
    for (; l > 0; l -= l & -l) ret -= BIT[l].query(forward<Args>(args)...);
    for (r++; r > 0; r -= r & -r) ret += BIT[r].query(forward<Args>(args)...);
    return ret;
  }
};

template <class T> struct FenwickTree<0, T> {
  T val; FenwickTree() : val(T()) {}
  void update(T v) { val += v; }
  T query() { return val; }
};
