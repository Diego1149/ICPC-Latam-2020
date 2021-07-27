/*
I. Impenetrable Wall

Solution by Diego1149

Solved with: DP O(N^3), Geometry
Difficulty: 7.5

Prerequisites: https://codeforces.com/blog/entry/48122 (Optional, but highly
recommended).

Let's start by ignoring G.

First of all we can notice that all of the points must be in increasing angle
according to the house, this is because otherwise we will have an area that
can't be seen by the house.

So we can sort the points in CCW (Counter Clockwise) order according to their
angle with the house.

Let's image one wall such that it fulfills all of the requirements.

In order to fulfill the requirement that the inner wall must have an
angle lesser than 180 degrees, we can notice that for every point in the wall,
let's call the point i, the point after (i+1) and and the point before (i-1)
must form a triangle(i-1, i, i+1) whose points are in CCW order.

Finally in order to guarantee that the house is inside of the points we know
that the triangle(h, i, i+1) must also be in CCW order.

If the problem were just that it can be solved with a DP in O(N^5), that is
because we need to keep the following parameters: i,j,f,s.

i: The last point to become part of the wall.
j: The second to last point to become part of the wall.
f: The first point to become part of the wall.
s: The second point to become part of the wall.

Note: We need f and s because otherwise we can't guarantee that the first point
has an angle lesser than 180 degrees.

In this solution we iterate over the sorted points over all points with index
bigger than j, let's call this index k and we check that the triangle(i,j,k) has
its points in CCW order and also that the triangle(h,j,k) has its points in CCW
order.

Since we have 4 dimensions of size N and we iterate over potentially N points
each time it becomes O(N^5).

This problem can be simplified though, that's because we know that G must always
be part of the wall, therefore we can force G to be the first point we
introduce, thus eliminating the need for f. Additionally, the problem mentions
that G doesn't need to have an inner angle lesser than 180 degrees, which means
we don't need to validate triangle(last,f,s), so we can eliminate s.

Finally, we arrive with a O(N^3) solution.

Note1: I use Oleksandr Bacherikov's implementation of point and vectors, which
greatly simplifies the implementation.
Note2: I treat the house as if it was the point (0,0) by translating every point
by the position of the house, that way code becomes simpler because I can sort
by angle according to the point (0,0) and check that the triangle(o,j,k), where
o is the origin (0,0) is in the CCW order.

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

#define maxN 301
#define MOD 1000000007

struct Point {
  ll x, y;
  Point operator-=(const Point &p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
};

int N;
Point h, g, p[maxN], tt[maxN];
ll dp[maxN][maxN];

bool isFirstHalf(const Point &p) { return p.y > 0 || p.y == 0 && p.x > 0; }

ll area(const Point &A, const Point &B, const Point &C) {
  return (C.y - A.y) * (B.x - A.x) - (B.y - A.y) * (C.x - A.x);
}

ll area(const Point &p1, const Point &p2) { return p1.x * p2.y - p1.y * p2.x; }

bool operator<(const Point &l, const Point &r) {
  if (isFirstHalf(l) != isFirstHalf(r)) return isFirstHalf(l);
  return area(l, r) > 0;
}

ll solve(int i, int j) {
  ll &c = dp[i][j];
  if (~c) return c;
  c = area(p[j], p[0]) > 0 && area(p[i], p[j], p[0]) > 0;
  FOR(k, j + 1, N) {
    if (j) {
      if (area(p[j], p[k]) <= 0 || area(p[i], p[j], p[k]) <= 0) continue;
    } else if (area(p[j], p[k]) <= 0)
      continue;
    c = (c + solve(j, k)) % MOD;
  }
  return c;
}

int main(int argc, char *argv[]) {
  _;
  while (scanf("%lld %lld", &h.x, &h.y) != EOF) {
    memset(dp, -1, sizeof(dp));
    scanf("%lld %lld", &p[0].x, &p[0].y);
    p[0] -= h;
    Point g = p[0];
    scanf("%d", &N);
    N++;
    FOR(i, 1, N) {
      scanf("%lld %lld", &p[i].x, &p[i].y);
      p[i] -= h;
    }
    sort(p, p + N);

    // Find the id of G, that way we can force G to be the first point, point
    // with index 0.
    int id = 0;
    FOR(i, 0, N) if (p[i].x == g.x && p[i].y == g.y) {
      id = i;
      break;
    }
    FOR(i, 0, N) tt[(i - id + N) % N] = p[i];
    FOR(i, 0, N) p[i] = tt[i];
    printf("%lld\n", solve(0, 0));
  }
  return 0;
}
