#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given N people, and a list of their ranked preferences for roommates,
//   the goal is to arrange N / 2 pairs such that if a person x prefers
//   a person y more than their current roommate, then person y prefers
//   their current roommate more than x
// Constructor Arguments:
//   prefs: a matrix of size N x (N - 1) with prefs[i][j] representing the jth
//     preferred choice for the person i 
//     prefs[i] must be a permutation of 0 to N - 1, excluding i
// Fields:
//   N: the number of people
//   mate: a vector representing the roommate of each person; all -1 if there
//     is no stable matching
// In practice, has a moderate constant
// Time Complexity:
//   constructor O(N^2)
// Memory Complexity: O(N^2)
// Tested:
//   https://codeforces.com/contest/1423/problem/A
struct StableRoommates {
  struct NoMatch {}; int N; vector<int> mate;
  StableRoommates(vector<vector<int>> prefs) : N(prefs.size()), mate(N, -1) {
    if (N % 2 == 1 || N <= 0) return;
    vector<vector<int>> rnk(N, vector<int>(N, 0));
    vector<int> fr(N, 0), bk(N, N - 1), proposed(N, -1);
    vector<vector<bool>> active(N, vector<bool>(N, true)); queue<int> q;
    auto rem = [&] (int i, int j) { active[i][j] = active[j][i] = false; };
    auto clip = [&] (int i) {
      while (fr[i] < bk[i] && !active[i][prefs[i][fr[i]]]) fr[i]++;
      while (fr[i] < bk[i] && !active[i][prefs[i][bk[i] - 1]]) bk[i]--;
      if (fr[i] >= bk[i]) throw NoMatch();
    };
    auto add = [&] (int i, int j) {
      proposed[mate[i] = j] = i; while (true) {
        clip(j); if (prefs[j][bk[j] - 1] != i) rem(j, prefs[j][bk[j] - 1]);
        else break;
      }
    };
    auto nxt = [&] (int i) {
      clip(i); int j = prefs[i][fr[i]++];
      clip(i); prefs[i][--fr[i]] = j; return proposed[prefs[i][fr[i] + 1]];
    };
    for (int i = 0; i < N; i++) {
      q.push(i); for (int j = 0; j < N - 1; j++) rnk[i][prefs[i][j]] = j;
    }
    try {
      while (!q.empty()) {
        int i = q.front(); q.pop(); while (true) {
          clip(i); int j = prefs[i][fr[i]], i2 = proposed[j];
          if (i2 != -1 && rnk[j][i2] < rnk[j][i]) { rem(i, j); continue; }
          if (i2 != -1) { mate[i2] = proposed[j] = -1; q.push(i2); }
          add(i, j); break;
        }
      }
      int cur = 0; while (true) {
        for (; cur < N; cur++) {
          clip(cur); if (bk[cur] - fr[cur] > 1) break;
        }
        if (cur == N) break;
        vector<int> cyc1, cyc2; int i = cur, j = i;
        do { i = nxt(i); j = nxt(nxt(j)); } while (i != j);
        do { cyc1.push_back(j); j = nxt(j); } while (i != j);
        for (int k : cyc1) {
          j = mate[k]; cyc2.push_back(j); mate[k] = proposed[j] = -1;
          rem(k, j);
        }
        for (int k = 0; k < int(cyc1.size()); k++)
          add(cyc1[k], cyc2[(k + 1) % cyc2.size()]);
      }
    } catch (NoMatch &) { fill(mate.begin(), mate.end(), -1); }
  }
};
