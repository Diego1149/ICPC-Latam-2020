/*
N. Non-Integer Donuts

Solution by Diego1149

Solved with: String parsing O(N)
Difficulty: 1

Parse the string and sum the cents, if the cents are multiple of 100 add 1 to
the counter. You can parse manually or with scanf.
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

int main(int argc, char *argv[]) {
  _;
  int N;
  while (scanf("%d\n", &N) != EOF) {
    int cc = 0, ans = 0;
    FOR(i, 0, N + 1) {
      double f;
      scanf("$%lf\n", &f);
      f -= (ll)f;
      cc += round(f * 100);
      if (i && cc % 100) ans++;
    }
    printf("%d\n", ans);
  }
  return 0;
}
