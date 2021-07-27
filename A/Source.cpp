/*
A. Almost Origami

Solution by Diego1149

Solved with: Brute force
Difficulty: 8

Prerequisites: Brute force knowledge and basic math.

Let's call the sets of points we can use in the left side L and the ones on the
right R.

First we can notice that since the sets L and R start being equal and everytime
we select 1 point from L and one from R the intersection will be at height H, we
know we will introduce the same height to both L and R, therefore we can have a
single set S which contains all heights.

We have to choose 2 Hs from the set and find the point where the lines would
cross if one is on the left and one on the right. For that we can first realize
that the width of the paper doesn't matter, the height at which the 2 lines
intersect will always be the same, so we can have a paper of dimensions 1x1.

With that said the line we chose from the height on the left going to the bottom
right will have the following formula:

y = -H[i]*x + H[i]

The line we form from the lower left corner to the right will have formula:

y = H[j]*x

We can simplify to:

H[j]*x = -H[i]*x + H[i]
H[j]*x + H[i]*x = H[i]
x*(H[j] + H[i]) = H[i]
x = H[i]/(H[j]+H[i])

Since we care about the height, which is y we can get the following by replacing
x in the previous formula:

y = H[i]*H[j]/(H[i]+H[j])

Next we can notice that Hs will be given as fractions, which means we can
replace H[i] with num[i]/den[i]:

y = (num[i]/den[i] * num[j]/den[j]) / (num[i]/den[i] + num[j]/den[j])

Rearranging the top part:
y = (num[i]*num[j] / (den[i]*den[j])) / (num[i]/den[i] + num[j]/den[j])

Expanding the fraction below
y = (num[i]*num[j] / (den[i]*den[j])) /
    ((num[i]*den[j] + num[j]*den[i])/den[i]*den[j])

We can cancel de denominators
y = num[i]*num[j] / (num[i]*den[j] + num[j]*den[i])

We can use that to produce the next numbers to introduce to H, with the top part
being the numerator and the bottom part the denominator. We can notice that in
the begining our set S only has a single H which is 1/1, so whatever we
introduce will have a numerator 1 and we know that 1*1 =1, therefore the
numerators will always be 1.

We can simplify to:
y = 1 / (den[j] + den[i])

In other words we can rewrite the problem to the following:
We have a set S (initially with the number 1), we can perform an operation on
othe set which is grab any 2 numbers of the set (potentially the same number)
and introduce the sum of both numbers to the set. What is the minimum amount of
operations we have to do to have number X in the set?

This problem sounds like it can be solved with DP, however we can realize that
if we establish the transitions in such way that we never visit the same state
twice, rendering the memoization useless.

To do that we will only introduce to the set numbers that are strictly bigger to
all numbers on the set, we can appretiate that in the beginnig we have: {1},
then we have {1,2}, after that we can branch to {1,2,3} or {1,2,4}, from the
latter we will never add 3, so the states will always be disjoint, same thing
happens for all branches going forward.

We can use brute force to solve that problem via a BFS or a DFS (with BFS we
stop once every number we care about has been reached), with DFS it's trickier
because we can keep expanding. So I found a simple heuristic which is the answer
for a number X will be smaller or equal to:

B + O - 2

Where B is the amount of bits in the number.
O is the amount of bits that are set to 1 in the number.

The reason for this formula is because we can keep adding all powers of 2 until
we reach the biggest power of 2 in X and then add every power where a bit is on.

Since N is max 100 we can notice that the answer will be smaller or equal to 10,
which means we can solve with a DFS and stop at depth 10.

Given that we don't know if the problem will run in time we can run it locally
to test how fast it is (which if your brute force is optimized it should
definitely be fast enough) or simply print all the answers and put them in a
precalculated array.
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

map<int, vi> mem;
vi cur;
int arr[11], cnt = 0;

void resolve() {
  if (cnt >= 10) return;
  int last = arr[cnt - 1];
  FOR(i, 0, cnt) {
    FOR(j, i, cnt) {
      int ne = arr[i] + arr[j];
      if (ne <= last || ne >= maxN) continue;
      cur.push_back(arr[i]);
      cur.push_back(arr[j]);
      arr[cnt++] = ne;
      if (mem.find(ne) == mem.end() || mem[ne].size() > cur.size()) {
        mem[ne] = cur;
      }
      resolve();
      cur.pop_back();
      cur.pop_back();
      cnt--;
    }
  }
}

int main(int argc, char* argv[]) {
  _;
  arr[0] = 1;
  cnt = 1;
  resolve();
  int M, N;
  while (cin >> M >> N) {
    if (M != 1) {
      cout << "*" << endl;
    } else {
      FOR(i, 0, mem[N].size()) { cout << "1 " << mem[N][i] << endl; }
    }
  }
  return 0;
}
