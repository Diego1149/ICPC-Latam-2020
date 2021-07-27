/*
L. Lola's Schedule

Solution by Diego1149

Solved with: DP O(N + T)
Difficulty: 2

Prerequisites: Very basic data structures or DP.

We can have an array of all times and for each time mark it if it's blocked.
This can be done in O(T), there are 2*10^5 possible times.

We can notice that if we are at position i we can only go to position i+X,
therefore we can do a linear DP where the current value is dp[i+X] and add 1 if
the current position is blocked.
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

#define maxT 200002
#define maxStart (60 * 8)

int N, X;
int dp[maxT];
bool op[maxT];

int main(int argc, char *argv[]) {
  _;
  while (scanf("%d %d", &N, &X) != EOF) {
    memset(op, false, sizeof(op));
    FOR(i, 0, N) {
      int s, d;
      scanf("%d %d", &s, &d);
      FOR(j, s, d + s + 1) op[j] = true;
    }
    int T = 0, C = INF;
    for (int i = maxT - 1; i >= 0; i--) {
      dp[i] = i + X < maxT ? dp[i + X] : 0;
      if (op[i]) dp[i]++;
      if (i <= maxStart) {
        if (C >= dp[i]) {
          C = dp[i];
          T = i;
        }
      }
    }
    printf("%d %d\n", T, C);
  }
  return 0;
}
