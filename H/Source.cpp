/*
H. Halting Wolf

Solution by Diego1149

Solved with: Maxflow (Dinic) O(E * min(sqrt(E), V^(2/3)) + N^3)
Difficulty: 5

Prerequisites: Maxflow, https://en.wikipedia.org/wiki/Dinic%27s_algorithm

Let's pretend that the requirement of an instruction getting executed infinite
amount of times doesn't exist. We can see that we have a graph where every edge
can be traversed once.

We need to identify everytime we pass through the beginning, in other words
we need to handle some sort of flow through the graph, so we can use maxflow to
solve it. In this maxflow the source and the sink are the same node (which isn't
normally allowed in maxflow), so for every node that points to the source we can
instead make it point to the sink.

We can notice that there are at most 10^4 edges, and also each edge is
unitary, therefore we don't need in the worst case V^2 iterations, but rather
the minimum between sqrt(E) and V^(2/3), since in the worst case sqrt(E) can be
bigger than V^(2/3), required iterations are less than 100, and we have 10^4
edges which means it can be solved in time.

Now regarding the infinite instructions, we can represent them as edges with
capacity 10^4+2, that is because we know that the previous graph will produce a
maxflow of at most 10^4+1 (using all edges, plus ending in the source and going
nowhere).

We can see that if we run Dinic in this graph and the answer is bigger than
10^4+2 there are infinite amount of solutions.

Regarding the time complexity, there are at most O(N) such edges, which means
that the Dinic's worst case of (V^2*E) becomes O(N^3).

As such we will require the sum of the previous iterations plus the iterations
that rely on infinite to solve the problem.

Note: The implementation that I'm using for Maxflow is compressing all edges
that start on node u and reach node v into a single edge from u to v with
capacity amount of such nodes. Worst case stays the same (but might be better).

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
#define maxT 10001

struct FlowEdge {
  int v, u;
  long long cap, flow = 0;
  FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
  const long long flow_inf = 1e18;
  vector<FlowEdge> edges;
  vector<vector<int>> adj;
  int n, m = 0;
  int s, t;
  vector<int> level, ptr;
  queue<int> q;

  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
    adj.resize(n);
    level.resize(n);
    ptr.resize(n);
  }

  void add_edge(int v, int u, long long cap) {
    edges.emplace_back(v, u, cap);
    edges.emplace_back(u, v, 0);
    adj[v].push_back(m);
    adj[u].push_back(m + 1);
    m += 2;
  }

  bool bfs() {
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int id : adj[v]) {
        if (edges[id].cap - edges[id].flow < 1) continue;
        if (level[edges[id].u] != -1) continue;
        level[edges[id].u] = level[v] + 1;
        q.push(edges[id].u);
      }
    }
    return level[t] != -1;
  }

  long long dfs(int v, long long pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;
    for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
      int id = adj[v][cid];
      int u = edges[id].u;
      if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
        continue;
      long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
      if (tr == 0) continue;
      edges[id].flow += tr;
      edges[id ^ 1].flow -= tr;
      return tr;
    }
    return 0;
  }

  long long flow() {
    long long f = 0;
    while (true) {
      fill(level.begin(), level.end(), -1);
      level[s] = 0;
      q.push(s);
      if (!bfs()) break;
      fill(ptr.begin(), ptr.end(), 0);
      while (long long pushed = dfs(s, flow_inf)) {
        f += pushed;
      }
    }
    return f;
  }
};

int mat[maxN][maxN];

int main(int argc, char* argv[]) {
  _;
  int N;
  while (scanf("%d", &N) != EOF) {
    memset(mat, 0, sizeof(mat));
    Dinic dd = Dinic(N + 1, 0, N);
    FOR(i, 0, N) {
      int k;
      char ss[6];
      scanf("%s", ss);
      if (ss[0] == '*') {
        int t;
        scanf("%d", &t);
        if (t == 1) t = N + 1;
        mat[i][t - 1] = maxT + 1;
      } else {
        k = atoi(ss);
        FOR(j, 0, k) {
          int t;
          scanf("%d", &t);
          if (t == 1) t = N + 1;
          mat[i][t - 1]++;
        }
      }
    }
    FOR(i, 0, N) {
      FOR(j, 0, N + 1) {
        if (!mat[i][j]) continue;
        dd.add_edge(i, j, mat[i][j]);
      }
    }
    int ans = dd.flow();
    if (ans > maxT) {
      printf("*\n");
    } else
      printf("%d\n", ans + 1);
  }
  return 0;
}
