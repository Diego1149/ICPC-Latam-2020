/*
E. Excellent Views

Solution by Diego1149

Solved with: Any structure that handles range updates in O(N*log(N)) or better.
Difficulty: 4

Prerequisites: Segment tree, etc.

We can notice that if we sort them by height (keeping track of the indices), and
then iterate over them in decreasing order, then we can get the ids of previous
heights that we have already added.

We would need to get the smaller id that is bigger to current id (idNext) and
the biggest id that is smaller than current id (idPrev).

If we do that, then we know that all buildings closer to current id can go to
that one, therefore for all those buildings we will increase their value by 1,
doing this would be O(N^2), we can optimize it by having any structure that
handles updating values by ranges.

In order to obtain the range it must be the average between id and idNext and
the average between id and idPrev, if there is a building at the same distance
from id and idPrev or idNext, we ignore it.

Don't forget to remove id from the range to be updated.

Note: I used a set for obtaining idNext and idPrev and a segment tree for
updating the ranges, but any structure works.
*/

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define INF 1000000000
#define FOR(i, a, b) for (int i = int(a); i < int(b); i++)
#define FORC(cont, it) \
  for (decltype((cont).begin()) it = (cont).begin(); it != (cont).end(); it++)
#define pb push_back
#define _                                               \
  if (argc == 2 && ((string)argv[1]) == "USE_IO_FILES") \
    freopen("in.txt", "r", stdin), freopen("out.txt", "w", stdout);

using namespace std;
using namespace __gnu_pbds;

typedef long long ll;
typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;
typedef vector<vi> vvi;
typedef tree<int,        // key
             null_type,  // value
             less<int>, rb_tree_tag, tree_order_statistics_node_update>
    ordered_set;
/*
  How to use ordered_set:
  ordered_set X;
  X.insert(1);
  X.erase(1);
  X.find_by_order(1); // This gives an iterator.
  X.order_of_key(-5); // Lower bound, 0 based position.
 */

#define maxN 100000

struct Data {
  int id, h;
  bool operator<(const Data &r) const { return h < r.h; }
};

struct Node {
  ll ans, lazy;
};

int N, D, h[maxN];
Data dd[maxN];
Node t[4 * maxN];

void propagate(int n) {
  if (n < D) {
    t[n << 1].lazy += t[n].lazy;
    t[n << 1 | 1].lazy += t[n].lazy;
  }
  t[n].ans += t[n].lazy;
  t[n].lazy = 0;
}

void updateRange(int n, int d, int lo, int hi, int v) {
  propagate(n);
  if (lo > hi) return;
  if (!lo && hi == d - 1) {
    t[n].lazy += v;
    propagate(n);
    return;
  }
  d >>= 1;
  updateRange(n << 1, d, lo, min(hi, d - 1), v);
  updateRange(n << 1 | 1, d, max(0, lo - d), hi - d, v);
}

int main(int argc, char *argv[]) {
  _;
  while (scanf("%d", &N) != EOF) {
    memset(t, 0, sizeof(t));
    D = 1 << (int)ceil(log2(N));
    FOR(i, 0, N) {
      scanf("%d", h + i);
      dd[i] = Data{i, h[i]};
    }
    sort(dd, dd + N);
    set<int> busy;
    busy.insert(-2 * N);
    busy.insert(4 * N);
    for (int i = N - 1; i >= 0; i--) {
      auto it = busy.insert(dd[i].id).first;
      auto it2 = ++it;
      it--;
      auto itp = --it;
      it++;
      int lo = *itp + *it >> 1;
      int hi = *it2 + *it + 1 >> 1;
      updateRange(1, D, max(0, lo + 1), min(hi - 1, N - 1), 1);
      if (lo + 1 <= hi - 1) updateRange(1, D, *it, *it, -1);
    }
    FOR(i, 1, 2 * D) propagate(i);
    FOR(i, D, D + N) printf("%lld%c", t[i].ans, " \n"[i == D + N - 1]);
  }
  return 0;
}
