/*
K. Keylogger

Solution by Diego1149

Solved with: DP (N*K*log(K))
Difficulty: 4

Prerequisites: Binary Search

We can notice that if we handle the state of (n,k) where n means that we have
processed the character at position n and k means that it was character k, then
we can iterate over j (all possible new values of k) and move to the ones that
fulfill: T[n][j] - L <= p[n] <= T[n][j] + L

That would mean the DP is solved in O(N*K^2), so we should optimize it. We can
realize that T[n][j] <= T[n][j+1], therefore we can do binary search on the
matrix to see which ones fulfill the requirement instead of iterating over all
of them.

If we do an upper_bound for p[n]+L, let's call it fi, we know that every value
lesser than fi fulfills T[n][j] - L <= p[n].

We can do a lower_bound for p[n]-L, let's call it la, we know that every value
bigger or equal to la fullfills p[n] < T[n][j] + L.

What we want is the intersection between fi and la, to do that we can do suffix
sums dp[n-1][j] and therefore the answer will be dp[n-1][fi-1] - dp[n-1][la-1].

Note: On my solution I do the suffix sums on the dp itself, that way I avoid
creating extra memory.
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

#define maxN 10000
#define maxK 750
#define MOD 1000000007

ll K, L, N;
ll T[maxK][maxK];
ll p[maxN];
ll dp[maxN][maxK];

int main(int argc, char *argv[]) {
  _;
  while (scanf("%lld %lld", &K, &L) != EOF) {
    FOR(i, 0, K) FOR(j, 0, K) scanf("%lld", T[i] + j);
    scanf("%lld", &N);
    FOR(i, 0, N - 1) scanf("%lld", p + i);
    FOR(k, 0, K) dp[N - 1][k] = k + 1;
    for (int n = N - 2; n >= 0; n--) {
      FOR(k, 0, K) {
        dp[n][k] = k ? dp[n][k - 1] : 0;
        int fi = upper_bound(T[k], T[k] + K, p[n] + L) - T[k];
        ll v1 = fi ? dp[n + 1][fi - 1] : 0;
        int la = lower_bound(T[k], T[k] + K, p[n] - L) - T[k];
        ll v2 = la ? dp[n + 1][la - 1] : 0;
        dp[n][k] = (dp[n][k] + v1 - v2 + MOD) % MOD;
      }
    }
    printf("%lld\n", dp[0][K - 1]);
  }
  return 0;
}
