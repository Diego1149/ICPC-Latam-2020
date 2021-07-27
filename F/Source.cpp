/*
F. Fascinating Partitions

Solution by Diego1149

Solved with: DP O(N^2)
Difficulty: 7

Prerequisites: DP, using a Stack to sort

Let's solve for the minimum, maximum is the same, but we grab max instead.

First we can notice that this can be solved with a DP[i][j] in O(N^3), we would
have to iterate over all k such that:
dp[i][j] = min(dp[i][j], dp[i-1][k] + max(j...k))

For this we can notice that it can be optimized, this is because for every value
j there are 2 posibilities, A[j] is the biggest element in the partition, or
it's not the biggest element in the parition.

If there is another element that is bigger, let's call the right-most index m;
therefore, dp[i][j] must be the minimum between dp[i][m] and the value we had on
the previous dp, it's just that we can stop at m instead of keep going to the
left.

A trick we can use to obtain these values is to notice that since for positions
bigger than m we know A[j] is the biggest, then we only care about the value of
the smallest dp.

We can use a stack where we will store a pair of numbers (X,Y) which means that
there is a number in A with value X and we know that before reaching the biggest
number to its left the minimum dp has value Y.

It's easy to notice that if we are in position j we must pop out of the stack
all numbers with X <= A[j], we will then push (A[j], lo1) into the stack. Where
lo1 is the minimum dp that has an index bigger than m but smaller than j. We can
obtain lo1 by getting the minimum between dp[i-1][j-1] and all the Ys we pop out
of the stack.

Note1: I coded this with linear memory, probably should have used O(N^2) memory
since the time complexity is O(N^2) anyways. In this memory optimization trick
we only keep track of the current dp and the previous dp so instead of accessing
dp[i] we access dp by the last bit of i.

Note2: Probably I could have reused the code for the minimum and maximum to make
the implementation cleaner.
*/

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define INF 1000000000000000000
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
typedef pair<ll, ll> pll;
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

#define maxN 8000

int N, a[maxN], p[maxN];
ll dp[2][maxN], lo[maxN];
pll q[maxN];

int main(int argc, char *argv[]) {
  _;
  while (scanf("%d", &N) != EOF) {
    FOR(i, 0, N) scanf("%d", a + i);
    FOR(j, 0, N) dp[1][j] = INF;
    FOR(i, 0, N) {
      int qc = 0;
      FOR(j, i, N) {
        ll lo1 = INF, mv = INF;
        if (j) lo1 = dp[~i & 1][j - 1];
        while (qc && a[q[qc - 1].first] < a[j]) lo1 = min(lo1, q[--qc].second);
        if (!qc) {
          if (!i) lo1 = 0;
        } else {
          mv = dp[i & 1][q[qc - 1].first];
        }
        q[qc++] = pll(j, lo1);
        dp[i & 1][j] = min(lo1 + a[j], mv);
      }
      lo[i] = dp[i & 1][N - 1];
    }

    FOR(j, 0, N) dp[1][j] = 0;
    FOR(i, 0, N) {
      int qc = 0;
      FOR(j, i, N) {
        ll lo1 = 0, mv = 0;
        if (j) lo1 = dp[~i & 1][j - 1];
        while (qc && a[q[qc - 1].first] < a[j]) lo1 = max(lo1, q[--qc].second);
        if (qc) {
          mv = dp[i & 1][q[qc - 1].first];
        }
        q[qc++] = pll(j, lo1);
        dp[i & 1][j] = max(lo1 + a[j], mv);
      }
      printf("%lld %lld\n", lo[i], dp[i & 1][N - 1]);
    }
  }
  return 0;
}
