#ifndef ALGORITHMS_QUERIES_MOUPDATES_H_
#define ALGORITHMS_QUERIES_MOUPDATES_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Mo's Algorithm supporting updates
 *
 * Decomposes the array into blocks of size N ^ (2 / 3) to answer queries.
 * Iterates though each pair of blocks and answers the relevant queries.
 *
 * Time complexity: Q * N ^ (2 / 3)
 */
class MoUpdates {
private:
    int res;
    int *A, *ans;
    unordered_map<int, int> cnt;
    vector<int> **q;
    stack<pair<int, int>> revert;

    inline void add(int a) {
        cnt[a]++;
        if (cnt[a] == 1) res++;
    }

    inline void remove(int a) {
        cnt[a]--;
        if (cnt[a] == 0) res--;
    }

    inline void update(int i, int a) {
        remove(A[i]);
        A[i] = a;
        add(A[i]);
    }

public:
    /**
     * Returns the answer to the ith query.
     *
     * @param i the index of the query
     * @return the answer the query
     */
    int query(int i) {
        return ans[i];
    }

    /**
     * Computes the number of unique values between l and r inclusive, provided the
     * queries are known beforehand.
     *
     * If isQuery[i] is true, then queries[i] contains 2 indices l and r, otherwise,
     * it contains and index i and an element a to change the A[i] to.
     *
     * @param a the array (must contain only positive values)
     * @param N the number of elements in the array
     * @param queries the array of pairs containing the queries
     * @param isQuery the array containing the type of query
     * @param Q the number of queries
     */
    MoUpdates(int *arr, int N, bool oneIndexed, pair<int, int> *queries, bool *isQuery, int Q) {
        A = new int[N + oneIndexed];
        for (int i = oneIndexed; i < N + oneIndexed; i++) {
            A[i] = arr[i];
        }
        int sz = (int) cbrt(N); sz *= sz;
        int blocks = (N + oneIndexed - 1) / sz + 1;
        q = new vector<int> *[blocks];
        for (int i = 0; i < blocks; i++) {
            q[i] = new vector<int>[blocks];
        }
        ans = new int[Q];
        res = 0;
        for (int i = 0; i < Q; i++) {
            if (isQuery[i]) q[queries[i].first / sz][queries[i].second / sz].push_back(i);
            else {
                for (int bl = 0; bl <= queries[i].first / sz; bl++) {
                    for (int br = queries[i].first / sz; br < blocks; br++) {
                        q[bl][br].push_back(i);
                    }
                }
            }
        }
        int l = oneIndexed, r = l - 1;
        for (int bl = 0; bl < blocks; bl++) {
            for (int br = bl; br < blocks; br++) {
                for (int i : q[bl][br]) {
                    if (isQuery[i]) {
                        while (l < queries[i].first) {
                            remove(A[l]);
                            l++;
                        }
                        while (l > queries[i].first) {
                            l--;
                            add(A[l]);
                        }
                        while (r < queries[i].second) {
                            r++;
                            add(A[r]);
                        }
                        while (r > queries[i].second) {
                            remove(A[r]);
                            r--;
                        }
                        ans[i] = res;
                    } else {
                        revert.push({queries[i].first, A[queries[i].first]});
                        if (l <= queries[i].first && queries[i].first <= r) {
                            update(queries[i].first, queries[i].second);
                        } else {
                            A[queries[i].first] = queries[i].second;
                        }
                    }
                }
                while (!revert.empty()) {
                    pair<int, int> x = revert.top();
                    revert.pop();
                    if (l <= x.first && x.first <= r) {
                        update(x.first, x.second);
                    } else {
                        A[x.first] = x.second;
                    }
                }
            }
        }
    }
};

#endif /* ALGORITHMS_QUERIES_MOUPDATES_H_ */
