/*
B. Beautiful Mountains

Solution by Diego1149

Solved with: Deterministic Finite Automaton O(N*log(N)), Sieve of Eratosthenes
Difficulty: 6

Prerequisites: Sieve of Eratosthenes.

We are asked if we can find beautiful partitions of mountains such that all have
the same size, except the last one which can be of arbitrary size as long as
it's smaller than the rest.

If the constraint about the last one being smaller was not present we could
solve it with a Deterministic Finite Automation (DFA) such that we know that all
mountains must be of size K.

Given that all mountains must be of the same size we can see that K must be a
divisor of N. Therefore we should check for all of the divisors of N and see if
there exists one such that can have all the partitions of size K.

The DFA can have the following states:

 0 -> After processing the first number of the mountain.
 1 -> We are increasing
 2 -> We are either increasing or decreasing
 3 -> We are decreasing
 4 -> Invalid

We iterate over the array of numbers for a partition of size K and the
transitions are established by comparing the last number that was known A[d]
with the current number A[i], therefore we have the following cases:

A[d] < A[i]:
We are increasing therefore:
0 -> 1,
1 -> 1,
2 -> 1,
3 -> 4, (it's invalid, we can't go down and then up)
4 -> 4,

A[d] > A[i]:
0 -> 4, (it's invalid, we can't go down after processing the first number, first
we must go up)
1 -> 3,
2 -> 3,
3 -> 3,
4 -> 4,

A[d] == A[i]:
0 -> 1,
1 -> 2,
2 -> 2,
3 -> 3,
4 -> 4,

Notice that in order to handle the -1s in the input, we treat them as the
A[d]==A[i] case.

This works in O(N)


Once we have that, we have to realize that since the tail can be of any size,
then we must iterate over all Ks such that the problem becomes O(N^2). In order
to optimize we can realize that there are certain things that make certain Ks
have invalid beautiful mountains, so we should remove all such numbers and not
consider them.

There are 3 things we can do:

Note: indices are is 1-indexed.

If both A[i] and A[i+1] are known and A[i] < A[i+1]:
This can't be the last 2 numbers of any sequence because it would be invalid,
therefore any sequence that ends in i+1 is an invalid K, so we know that all
divisors of i+1 must also be invalid.

If both A[i] and A[i+1] are known and A[i] > A[i+1]:
This can't be the first 2 numbers of any sequence because it would be invalid,
therefore any sequence that begins in i is an invalid K, which means i-1 can't
be a valid end, so we know that all divisors of i-1 must also be invalid.

We have a sequence such that A[p] > A[d] < A[i]:
Where d is the index of the last known number and p is the index of the last
known number bigger than A[d].

Any K that contains all the indexes from p to i at the same time is invalid,
therefore we can make the K that is equal to i invalid and also all of its
divisors that are bigger or equal to i-p+1.

When we move to i+1 we do the same thing, but now for all the divisors of i+1
that are greater or equal to i+1-p+1. We do the same for all subsequent i's.
In this last case we can notice that if we find another sequence with a bigger
p, we can start using that because since it's smaller it means that we can
include the same divisors as with previous p, but also might include smaller
ones.

Due to sieve of erathostenes this works in O(N*log(N)) since that's the total
amount of divisors if we get the sum of all divisors for all numbers from 1 to
N.

This almost works, it's missing one last thing, validate the tail, to do that we
can run the initial DFA (from the end towards the beginning) and on every state
that is not 2 or 3 (valid states) we mark it as invalid, which means if we are
in position i we make every divisor or i-1 invalid.

Note: To solve this I did 1 single function and run the DFA and the 3 checks in
there. I call the function with the array and the reversed array.

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

#define maxN 100001

int N, A[maxN], valid[maxN];
vi divisors[maxN];

void mark(int j, int y) {
  FOR(i, 0, divisors[j].size()) {
    int div = divisors[j][i];
    if (div >= y) valid[div] = false;
  }
}

void check(int inverted) {
  int p = -1, d = -1, state = 0, inv = -1;
  FOR(i, 1, N) {
    // Sates:
    // 0 -> After processing the first number
    // 1 -> We are increasing
    // 2 -> We are increasing but might be decreasing as well
    // 3 -> We are decreasing
    // 4 -> Invalid
    int pv = d == -1 ? A[i] : A[d];
    if (A[i] == -1 || A[i] == pv) {
      if (state == 0)
        state = 1;
      else if (state == 1 || state == 2)
        state = 2;
      else if (state == 3)
        state = 3;
    } else if (A[i] > pv) {
      if (state <= 2)
        state = 1;
      else
        state = 4;
    } else if (A[i] < pv) {
      if (state == 0 || state == 4)
        state = 4;
      else
        state = 3;
    }
    if (state != 2 && state != 3) {
      if (!inverted)
        mark(i + 1, i + 1);
      else
        mark(N - i - 1, i + 1);
    }

    // 2 consecutives defined
    if (!inverted && A[i] != -1 && A[i - 1] != -1) {
      if (A[i] > A[i - 1]) {
        mark(i + 1, 0);
      } else if (A[i] < A[i - 1]) {
        mark(i - 1, 0);
      }
    }

    // Invalid sequence by going down then up.
    if (A[i] != -1) {
      if (d != -1 && A[d] > A[i]) {
        p = d;
      } else if (d != -1 && p != -1 && A[i] > A[d]) {
        inv = p;
      }
      d = i;
    }
    if (inv != -1) {
      if (!inverted)
        mark(i + 1, i - inv + 1);
      else
        mark(N - i - 1, i + 1);
    }
  }
}

int main(int argc, char *argv[]) {
  _;
  FOR(i, 1, maxN) for (int j = 0; j < maxN; j += i) divisors[j].push_back(i);
  while (scanf("%d", &N) != EOF) {
    memset(valid, true, sizeof(valid));
    FOR(i, 0, N) scanf("%d", A + i);
    check(false);
    int lo = 0, hi = N - 1;
    while (lo < hi) swap(A[lo++], A[hi--]);
    check(true);
    bool v = false;
    FOR(i, 3, N + 1) v |= valid[i];
    printf("%c\n", v ? 'Y' : 'N');
  }
  return 0;
}
