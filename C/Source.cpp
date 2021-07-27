/*
C. Crisis at the Wedding

Solution by Diego1149

Solved with: DP O(N*log(N)) or O(N)
Difficulty: 4

Prerequisites: Any structure that does efficient queries in ranges.

Since the sum of glasses is divisible by N we know all will have the same amount
of glasses, we could, if we wanted to remove that average from every glass and
that way if we end up with a negative number, it means that it should have more
glasses otherwise that it should have less.

We can solve it from right to left and then invert the array and solve it from
right to left again to mimick going the other way around.

What we will do to solve it is we can keep a dp[i] with a pair of values (X,Y).
Where X is the amount of leftover glasses and Y is the current cost paid so far
to push the leftovers to the next dp (this could also be the cost so far to
reach position i, implementation is minimally different).

We can see that the value of dp[i] is going to be:
dp[i].X = dp[i-1].X + G[i]
dp[i].Y = dp[i-1].Y + dp[i].X

Doing this will solve the problem only assuming we start at the position 0,
since we can start at potentially any position we can create an exact copy of
the array G and append it to G, then do this exact DP, however now instead of
caring about the answer of dp[i], we only care about the answers where i>=N and
we want to remove everything we had so far when we reached i-N.

In other words the answer will be dp[i].Y - dp[i-N].Y - dp[i-N].X*N

There is a slight problem, and that is that it's possible that if we do this
that at some point in time we will end up with a negative number, so what we can
do to solve that is get the minimum between the range (i-N, i] and see if that
ever becomes negative, that is check if we remove dp[i-N].X from the minimum Y
in the range does it becomes negative. We can use any tree that does query in
ranges to solve it.

Note: My implementation is O(N) because I use a trick, since we know that the
range is always the same we can use a dequeue to keep only the smallest numbers,
that is we pop from the back if the current number is smaller and we will pop
from the front if that number comes from an id smaller than i-N.
*/

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define INF 1000000000000000000LL
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

#define maxN 100000

int g[maxN], N, vv;
pll v[2 * maxN];

ll process() {
  ll mc = INF;
  deque<pll> los;
  v[0] = pll(g[0] - vv, g[0] - vv);
  los.push_back(pll(0, g[0] - vv));
  FOR(i, 1, 2 * N) {
    // DP
    v[i].first = v[i - 1].first + g[i % N] - vv;
    v[i].second = v[i - 1].second + v[i].first;

    // Deque to keep track of the minimum in the range
    while (los.size() && los.back().second >= v[i].first) los.pop_back();
    los.push_back(pll(i, v[i].first));
    if (los.front().first <= i - N) los.pop_front();

    // Potential answers
    if (i >= N) {
      ll st = v[i - N].first;
      ll lo = los.front().second;
      if (lo < st) continue;
      mc = min(mc, v[i].second - v[i - N].second - st * N);
    }
  }
  return mc;
}

int main(int argc, char *argv[]) {
  _;
  while (scanf("%d", &N) != EOF) {
    ll ss = 0;
    FOR(i, 0, N) {
      scanf("%d", g + i);
      ss += g[i];
    }
    vv = ss / N;
    ll mc = process();
    int lo = 0, hi = N - 1;
    while (lo < hi) swap(g[lo++], g[hi--]);
    mc = min(mc, process());
    printf("%lld\n", mc);
  }
  return 0;
}
