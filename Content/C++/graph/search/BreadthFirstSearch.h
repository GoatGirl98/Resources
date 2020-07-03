#pragma once
#include <bits/stdc++.h>
using namespace std;

// Breadth First Traversal of a graph (weighted or unweighted)
// Vertices are 0-indexed
// contructor accepts a generic graph data structure (weighted or unweighted)
//   with the [] operator (const) defined to iterate over the adjacency list
//   (which is a list of ints for an unweighted graph, or a list of
//   pair<int, T> for a weighted graph with weights of type T), as well as a
//   member function size() (const) that returns the number of vertices in the
//   graph, a single source vertex, or a list of sources,
//   and a value for infinity
// dist[v] is the distance from the closest source vertex to vertex v,
//   or INF if unreachable, and is also the shortest distance for
//   an unweighted graph
// par[v] is the parent vertex for vertex v in the breadth first search tree
// getPath(v) returns the list of vertices on the path from the closest source
//   vertex to vertex v
// In practice, constructor has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
//   getPath: O(V)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/ddrp3
template <class T = int> struct BFS {
  vector<T> dist; vector<int> par; T INF;
  int getTo(int e) { return e; }
  T getWeight(int) { return 1; }
  int getTo(const pair<int, T> &e) { return e.first; }
  T getWeight(const pair<int, T> &e) { return e.second; }
  template <class Graph> BFS(const Graph &G, const vector<int> &srcs,
                             T INF = numeric_limits<T>::max())
      : dist(G.size(), INF), par(G.size(), -1), INF(INF) {
    vector<int> q(G.size(), -1); int front = 0, back = 0;
    for (int s : srcs) dist[q[back++] = s] = T();
    while (front < back) {
      int v = q[front++]; for (auto &&e : G[v]) {
        int w = getTo(e); if (dist[w] == INF)
          dist[q[back++] = w] = dist[par[w] = v] + getWeight(e);
      }
    }
  }
  template <class Graph> BFS(const Graph &G, int s,
                             T INF = numeric_limits<T>::max())
      : BFS(G, vector<int>(1, s), INF) {}
  vector<int> getPath(int v) {
    vector<int> path; for (; v != -1; v = par[v]) path.push_back(v);
    reverse(path.begin(), path.end()); return path;
  }
};
