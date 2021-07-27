/*
D. Dividing Candy

Solution by Diego1149

Solved with: O(N)
Difficulty: 2

Prerequisites: Basic Bit knowledge

We have to see if it's possible to divide it in such a way that 2 people have
powers of 2, since we get powers of 2 as input.

We can use a manual representation of the number in binary and then check if
there are exactly 2 bits turned on there is an answer, otherwise if there is 1
bit turned on and there are 2 or more numbers as input there is an answer as
well.

We can check the current position, if it's on we turn it off and go to the next
one, etc. Until it's not turned on and we turn it on. We can see that it becomes
amortized constant because if a bit is turned on N times, the next bit will be
turned on N/2 times, the next N/4, etc.
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

#define maxN 200001

int N, d[maxN];

int main(int argc, char *argv[]) {
  _;
  while (scanf("%d", &N) != EOF) {
    memset(d, 0, sizeof(d));
    FOR(i, 0, N) {
      int t;
      scanf("%d", &t);
      while (d[t]) d[t++] = 0;
      d[t] = 1;
    }
    int cc = 0;
    FOR(i, 0, maxN) if (d[i]) cc++;
    if (cc == 2 || cc == 1 && N != 1)
      printf("Y\n");
    else
      printf("N\n");
  }
  return 0;
}
