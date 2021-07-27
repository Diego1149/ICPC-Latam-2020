/*
J. Job Allocator

Solution by Diego1149

Solved with: Custom Data Structure O(N*2^K)
Difficulty: 5.5

Prerequisites: Understanding arrays and matrices.

First let's solve the problem for K=2, then we can move onto a generalization
for K=8.

Since the queries of type 'J' care about all CPUs that have all values bigger or
equal to what the query has, then we can notice that if we have a structure
cnts[A][B], which has all of the precomputed cpus with values bigger or equal to
both A and B, then we can solve the query of type 'J' in O(K) by just accessing
the matrix.

To update this matrix, if a cpu comes with values (X,Y), we would need to update
every cnts[x][y] and increase the value by 1 where 0 <= x <= X and 0 <= y <= Y.

Something to notice is that R is at most 8, in other words, X + Y <= 8.

The amount of operations we will need is (X+1)*(Y+1), the highest value we can
get is for X=4 and Y=4, which would be 25 operations.

For doing the delete we should do the exact same as when adding it it's just
that we decrease the value by 1 instead of adding 1.

In the real problem K=8, the only thing that changes is that cnts is not a
2-dimensional matrix, but rather an 8-dimensional matrix. R is still maximum
8, so the biggest value we could get is all X,Y,Z,... =1, which would mean 2^8
operations.

Note: In my implementation I don't declare cnts as an 8-dimentional matrix,
because it requires typing all 8 dimensions to access the values, so I thought
it would be cleaner to delcare it as an array and for every dimension give it
the value i*9^d, where d is the dimension index.

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
#define maxR 8
#define maxCombs 43046721

int N, R, comps[maxN][maxR], cnts[maxCombs], pot9[maxR];

void put(int *arr, int cnt, int v, int acum) {
  if (cnt >= R) {
    cnts[acum] += v;
    return;
  }
  FOR(i, 0, arr[cnt] + 1) put(arr, cnt + 1, v, acum + pot9[cnt] * i);
}

int main(int argc, char *argv[]) {
  _;
  pot9[0] = 1;
  FOR(i, 1, maxR) pot9[i] = pot9[i - 1] * 9;
  while (scanf("%d %d", &N, &R) != EOF) {
    int id = 0;
    memset(comps, 0, sizeof(comps));
    memset(cnts, 0, sizeof(cnts));
    char ch[2];
    FOR(i, 0, N) {
      scanf("%s", ch);
      if (ch[0] == 'C') {
        int r;
        scanf("%d", &r);
        FOR(j, 0, r) {
          int t;
          scanf("%d", &t);
          comps[id][t - 1]++;
        }
        put(comps[id++], 0, 1, 0);
      } else if (ch[0] == 'D') {
        int t;
        scanf("%d", &t);
        put(comps[t - 1], 0, -1, 0);
      } else {
        int r, acum = 0, vv[maxR];
        memset(vv, 0, sizeof(vv));
        scanf("%d", &r);
        FOR(j, 0, r) {
          int t;
          scanf("%d", &t);
          vv[t - 1]++;
        }
        FOR(i, 0, maxR) { acum += pot9[i] * vv[i]; }
        printf("%d\n", cnts[acum]);
      }
    }
  }
  return 0;
}
