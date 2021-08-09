#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum spanning tree (or forest) using Prims's algorithm
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   G: a generic weighted graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of pair<int, T> with weights of type T)
//       size() const: returns the number of vertices in the graph
//   INF: a value for infinity
// Fields:
//   mstWeight: the weight of the mst
//   mstEdges: a vector of tuples of the edges in the mst
// In practice, has a small constant, faster than Boruvka, slower that Kruskal
// Time Complexity:
//   constructor: O((V + E) log E)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/minspantree
template <class T> struct PrimMST {
  using Edge = tuple<int, int, T>; T mstWeight; vector<Edge> mstEdges;
  struct Node {
    T d; int v; Node(T d, int v) : d(d), v(v) {}
    bool operator < (const Node &o) const { return d > o.d; }
  };
  template <class WeightedGraph>
  PrimMST(const WeightedGraph &G, T INF = numeric_limits<T>::max())
      : mstWeight() {
    int V = G.size(); vector<bool> done(V, false);
    vector<T> mn(V, INF); vector<int> to(V, -1); std::priority_queue<Node> PQ;
    for (int s = 0; s < V; s++) if (!done[s]) {
      PQ.emplace(mn[s] = T(), s); while (!PQ.empty()) {
        int v = PQ.top().v; PQ.pop(); if (done[v]) continue;
        done[v] = true;
        for (auto &&e : G[v]) if (!done[e.first] && e.second < mn[e.first]) {
          to[e.first] = v; PQ.emplace(mn[e.first] = e.second, e.first);
        }
      }
    }
    for (int v = 0; v < V; v++) if (to[v] != -1) {
      mstEdges.emplace_back(v, to[v], mn[v]); mstWeight += mn[v];
    }
  }
};
