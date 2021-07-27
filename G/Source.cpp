/*
G. Game of Slots

Solution by Diego1149

Solved with: DP O(N^3)
Difficulty: 8

Prerequisites: DP, expected value, probabilities, basic game theory knowledge.

Since N is rather small (N <= 100) compared to the range, we can assume that all
numbers are different, the reason for that is that the probability of all of
them being different has to be bigger than:
((10^18 - 200) / 10^18)^200 = 0.99999999999996

That number has 13 9s, which means that it shouldn't affect the answer given
that we care about 9 digits of precision.



Bob Optimal Strategy: Iterate over the slots in decreasing order, always use
his smaller number that is bigger than Alice's current number to cover Alice's
number. If Bob doesn't have a number that is bigger than that number, then use
his smallest number.

Proof: If Bob can use a number to cover 1 of 2 slots, Bob maximizes points by
covering the one that gives most points.



Alice optimal strategy: Put them in increasing order.

Proof:
If we generate all permutations for Alice, let's call an arbitrary permutation
P.

Let's call Q a permutation that Bob would use to defeat Alice's arbitrary
permutation.

Let's grab any pair of numbers (i,j), where i is worth i points and j is worth j
points.

Now let's generate all permutations of said pairs.

We have the following 6 possibilities:
Posibility    Points if P[i] < P[j]     Points if P[i] > P[j]
P P Q Q       0                         0
Explanation: Both Q[i] and Q[j] are bigger than P[i] and P[j], so order doesn't
matter Alice always scores 0 points.

Posibility    Points if P[i] < P[j]     Points if P[i] > P[j]
P Q P Q       0                         0
Explanation: It doesn't matter how you order them Bob will use which ever is
bigger to cover whichever is bigger and his smaller one to cover your smaller
one and take all points.

Posibility    Points if P[i] < P[j]     Points if P[i] > P[j]
P Q Q P       j                         i
Explanation: Bob can only cover the smaller one, so wherever Alice puts the
bigger between P[i] and P[j] she will win that amount of points. It's better to
put the bigger in position j to get the most amount of points.

Posibility    Points if P[i] < P[j]     Points if P[i] > P[j]
Q P P Q       i                         i
Explanation: Bob has 1 value that is bigger than both P[i] and P[j], so he will
always use it to cover whatever you set in j, that way he maximises the amount
of points he gets.

Posibility    Points if P[i] < P[j]     Points if P[i] > P[j]
Q P Q P       j                         i
Explanation: Bob can only cover one, so he will cover whichever he can, it's
better for Alice to use the big one in position j and maximize the amount of
points.

Posibility    Points if P[i] < P[j]     Points if P[i] > P[j]
Q Q P P       i + j                     i + j
Explanation: Bob can't cover either, so bob will always get maximum amount of
points.

Based on that we know that given a permutation P, for any pair of integers (i,j)
if P[i] > P[j] we should swap P[i] and P[j], thus the optimal ordering is in
increasing order.




Given that we now know the strategies, how do we solve the problem?

It seems like we can solve this by doing a dp[n][m], where n means that Alice
has n cards left and Bob has m cards left. That will work fine for Alice's
strategy since she always does them in order; however, not for Bob, for Bob we
need to know how many cards Bob has used that are strictly bigger than Alice's.
We can add an extra parameters k and we end up with a dp[n][m][k].

Every time Alice sets a card we go to dp[n-1][m][k-1].
Every time Bob sets a card we go to dp[n][m-1][k+1].

Note: If Bob has used 0 cards that are strictly greater than Alice's cards then
we go to dp[n-1][m][0] and Alice will win the points so we increase Alice's
expected value by n (because Alice's cards are in order card in position n is
worth n points if that card is bigger than Bob's).

Since we are talking about expected value, multiply everything by the
probability of it happening, which for Alice having the next biggest card is
n/(n+m), for Bob it's m/(n+m).

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

#define maxN 101

int N;
double dp[maxN][maxN][maxN];

double solve(int n, int m, int k) {
  if (n < 0 || m < 0) return 0;
  if (n == 0 && m == 0) return 0;
  double &c = dp[n][m][k];
  if (c != -1) return c;
  double p1 = 1. * n / (n + m);
  c = p1 * solve(n - 1, m, max(0, k - 1)) + (1. - p1) * solve(n, m - 1, k + 1);
  if (!k) c += p1 * n;
  return c;
}

int main(int argc, char *argv[]) {
  _;
  FOR(i, 0, maxN) FOR(j, 0, maxN) FOR(k, 0, maxN) dp[i][j][k] = -1;
  while (scanf("%d", &N) != EOF) {
    printf("%.6f\n", solve(N, N, 0));
  }
  return 0;
}
