#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top down dynamic segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   IndexType: the type of the index of the array
//   LAZY: boolean to indicate whether or not range updates are enabled
//   PERSISTENT: boolean to indicate whether or not updates are persistent
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static ldef(): only required if LAZY is true, returns the lazy
//         default value of type Lazy
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): only required if LAZY is false, returns the
//         value r of type Lazy applied to l of type Data, must be associative
//       static applyLazy(l, r, k): only required if LAZY is true, returns the
//         value r of type Lazy applied to l of type Data over a segment of
//         length k, must be associative
//       static mergeLazy(l, r): only required if LAZY is true, returns
//         the values l of type Lazy merged with r of type Lazy,
//         must be associative
//       static getSegmentVdef(k): returns the default value over a segment of
//         length k
//     Sample Struct: C1 supports point assignments and range sum queries,
//         where the default value of each index is 1, C2 supports 
//         range assignments and range sum queries, where the default value of
//         each index is 1
//       struct C1 {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data applyLazy(const Data &l, const Lazy &r) { return r; }
//         template <class IndexType> static Data getSegmentVdef(IndexType k) {
//           return k;
//         }
//       };
//       struct C2 {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Lazy ldef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         template <class IndexType>
//         static Data applyLazy(const Data &l, const Lazy &r, int k) {
//           return r * k;
//         }
//         static Lazy mergeLazy(const Lazy &l, const Lazy &r) { return r; }
//         template <class IndexType> static Data getSegmentVdef(IndexType k) {
//           return k;
//         }
//       };
// Constructor Arguments:
//   N: the size of the array
//   A: a vector of type C::Data
// Functions:
//   update(i, v, newRoot): updates the index i with the lazy value v
//     and creates a new version if newRoot is true
//   update(i, v, newRoot): only valid if LAZY is true, updates
//     the range [l, r] with the lazy value v and creates
//     a new version if newRoot is true
//   query(l, r, rootInd): queries the range [l, r] for the version rootInd (or
//     the latest version of rootInd is -1), and returns the aggregate value
//   bsearchPrefix(l, r, f, rootInd): returns the smallest index i in the
//     range [l, r] for the version rootInd (or the latest version of rootInd
//     is -1) such that f(query(l, i)) returns true or r + 1 if none exist
//   bsearchSuffix(l, r, f, rootInd): returns the largest index i in the
//     range [l, r] for the version rootInd (or the latest version of rootInd
//     is -1) such that f(query(i, r)) returns true of l - 1 if none exist
//   revert(rootInd): creates a new version based off of version rootInd
//   reserveNodes(k): reserves space for k nodes in the dynamic segment tree
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1) for size constructor,
//                O(N) for iteartor and generating function constructors
//   update, query, bsearchPrefix, bsearchSuffix: O(log N) amortized unless
//     reserveNodes is called beforehand
// Memory Complexity: O(U log N) for U updates for single argument constructor,
//                    O(N + U log N) for two argument constructor 
// Tested:
//   https://dmoj.ca/problem/ccc05s5 (LAZY = false, PERSISTENT = false)
//   https://dmoj.ca/problem/wc18c4s4 (LAZY = false, PERSISTENT = true,
//     bsearchPrefix, bsearchSuffix)
//   https://codeforces.com/contest/1080/problem/F
//     (LAZY = false, PERSISTENT = true)
//   https://codeforces.com/contest/915/problem/E
//     (LAZY = true, PERSISTENT = false)
//   https://www.spoj.com/problems/TTM/ (LAZY = true, PERSISTENT = true)
template <class IndexType, const bool LAZY, const bool PERSISTENT, class C>
struct DynamicSegmentTree {
#define lazy_def template <const bool _ = LAZY> typename enable_if<_>::type
#define agg_def template <const bool _ = LAZY> typename enable_if<!_>::type
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  template <const bool _, const int __ = 0> struct Node {
    Data val; int l, r; Node(const Data &val) : val(val), l(-1), r(-1) {}
  };
  template <const int __> struct Node<true, __> {
    Data val; Lazy lz; int l, r;
    Node(const Data &val) : val(val), lz(C::ldef()), l(-1), r(-1) {}
  };
  IndexType N; vector<int> roots; vector<Node<LAZY>> TR;
  int makeNode(int cp, IndexType tl, IndexType tr) {
    if (~cp) TR.push_back(TR[cp]);
    else TR.emplace_back(C::getSegmentVdef(tr - tl + 1));
    return int(TR.size()) - 1;
  }
  lazy_def apply(int x, IndexType tl, IndexType tr, const Lazy &v) {
    TR[x].val = C::applyLazy(TR[x].val, v, tr - tl + 1);
    TR[x].lz = C::mergeLazy(TR[x].lz, v);
  }
  agg_def apply(int x, IndexType, IndexType, const Lazy &v) {
    TR[x].val = C::applyLazy(TR[x].val, v);
  }
  lazy_def propagate(int x, IndexType tl, IndexType tr) {
    if (TR[x].lz != C::ldef()) {
      IndexType m = tl + (tr - tl) / 2; if (!~TR[x].l || PERSISTENT) {
        int nl = makeNode(TR[x].l, tl, m); TR[x].l = nl;
      }
      if (!~TR[x].r || PERSISTENT) {
        int nr = makeNode(TR[x].r, m + 1, tr); TR[x].r = nr;
      }
      apply(TR[x].l, tl, m, TR[x].lz); apply(TR[x].r, m + 1, tr, TR[x].lz);
      TR[x].lz = C::ldef();
    }
  }
  agg_def propagate(int, IndexType, IndexType) {}
  int build(const vector<Data> &A, IndexType tl, IndexType tr) {
    int x = makeNode(-1, tl, tr);
    if (tl == tr) { TR[x].val = A[tl]; return x; }
    IndexType m = tl + (tr - tl) / 2; int nl = build(A, tl, m); TR[x].l = nl;
    int nr = build(A, m + 1, tr); TR[x].r = nr;
    TR[x].val = C::merge(TR[TR[x].l].val, TR[TR[x].r].val); return x;
  }
  int update(int y, IndexType tl, IndexType tr, IndexType l, IndexType r,
             const Lazy &v, int trSz) {
    int x = !~y || (PERSISTENT && y < trSz) ? makeNode(y, tl, tr) : y;
    if (l <= tl && tr <= r) { apply(x, tl, tr, v); return x; }
    propagate(x, tl, tr); IndexType m = tl + (tr - tl) / 2;
    if (tl <= r && l <= m) {
      int nl = update(TR[x].l, tl, m, l, r, v, trSz); TR[x].l = nl;
    }
    if (m + 1 <= r && l <= tr) {
      int nr = update(TR[x].r, m + 1, tr, l, r, v, trSz); TR[x].r = nr;
    }
    TR[x].val = C::merge(~TR[x].l ? TR[TR[x].l].val
                                  : C::getSegmentVdef(m - tl + 1),
                         ~TR[x].r ? TR[TR[x].r].val
                                  : C::getSegmentVdef(tr - m));
    return x;
  }
  Data query(int x, IndexType tl, IndexType tr, IndexType l, IndexType r) {
    if (r < tl || tr < l) return C::qdef();
    if (!~x) return C::getSegmentVdef(min(r, tr) - max(l, tl) + 1);
    if (l <= tl && tr <= r) return TR[x].val;
    propagate(x, tl, tr); IndexType m = tl + (tr - tl) / 2;
    return C::merge(query(TR[x].l, tl, m, l, r),
                    query(TR[x].r, m + 1, tr, l, r));
  }
  template <class F>
  int bsearchPrefix(int y, IndexType tl, IndexType tr,
                    IndexType l, IndexType r, Data &agg, F f, IndexType &ret) {
    if (r < tl || tr < l) { ret = r + 1; return y; }
    int x = !~y ? makeNode(y, tl, tr) : y; if (tl != tr) propagate(x, tl, tr);
    if (l <= tl && tr <= r) {
      Data v = C::merge(agg, TR[x].val);
      if (!f(v)) { agg = v; ret = r + 1; return x; }
    }
    if (tl == tr) { ret = tl; return x; }
    int m = tl + (tr - tl) / 2;
    int nl = bsearchPrefix(TR[x].l, tl, m, l, r, agg, f, ret);
    TR[x].l = nl; if (ret <= r) return x;
    int nr = bsearchPrefix(TR[x].r, m + 1, tr, l, r, agg, f, ret);
    TR[x].r = nr; return x;
  }
  template <class F>
  int bsearchSuffix(int y, IndexType tl, IndexType tr,
                    IndexType l, IndexType r, Data &agg, F f, IndexType &ret) {
    if (r < tl || tr < l) { ret = l - 1; return y; }
    int x = !~y ? makeNode(y, tl, tr) : y; if (tl != tr) propagate(x, tl, tr);
    if (l <= tl && tr <= r) {
      Data v = C::merge(agg, TR[x].val);
      if (!f(v)) { agg = v; ret = l - 1; return x; }
    }
    if (tl == tr) { ret = tl; return x; }
    int m = tl + (tr - tl) / 2;
    int nr = bsearchSuffix(TR[x].r, m + 1, tr, l, r, agg, f, ret);
    TR[x].r = nr; if (l <= ret) return x;
    int nl = bsearchSuffix(TR[x].l, tl, m, l, r, agg, f, ret);
    TR[x].l = nl; return x;
  }
  DynamicSegmentTree(const vector<Data> &A) : N(A.size()) {
    if (N > 0) {
      reserveNodes(N * 2 - 1); roots.push_back(build(A, 0, N - 1));
    }
  }
  DynamicSegmentTree(IndexType N) : N(N) { roots.push_back(-1); }
  lazy_def update(IndexType l, IndexType r, const Lazy &v, bool newRoot) {
    int nr = update(roots.back(), 0, N - 1, l, r, v, TR.size());
    if (newRoot) roots.push_back(nr);
    else roots.back() = nr;
  }
  void update(IndexType i, const Lazy &v, bool newRoot) {
    int nr = update(roots.back(), 0, N - 1, i, i, v, TR.size());
    if (newRoot) roots.push_back(nr);
    else roots.back() = nr;
  }
  Data query(IndexType l, IndexType r, int rootInd = -1) {
    return query(~rootInd ? roots[rootInd] : roots.back(), 0, N - 1, l, r);
  }
  template <class F>
  IndexType bsearchPrefix(int l, int r, F f, int rootInd = -1) {
    Data agg = C::qdef(); IndexType ret = r + 1;
    int &root = ~rootInd ? roots[rootInd] : roots.back();
    root = bsearchPrefix(root, 0, N - 1, l, r, agg, f, ret);
    return ret;
  }
  template <class F>
  IndexType bsearchSuffix(int l, int r, F f, int rootInd = -1) {
    Data agg = C::qdef(); IndexType ret = l - 1;
    int &root = ~rootInd ? roots[rootInd] : roots.back();
    root = bsearchSuffix(root, 0, N - 1, l, r, agg, f, ret);
    return ret;
  }
  void revert(int rootInd) { roots.push_back(roots[rootInd]); }
  void reserveNodes(int k) { TR.reserve(k); }
#undef lazy_def
#undef agg_def
};
