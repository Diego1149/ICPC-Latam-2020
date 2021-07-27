/*
M. May I Add a Letter?

Solution by Diego1149

Solved with: Suffix Automaton, Persistent Data Structures O(N)
Difficulty: 10

Prerequisites: Suffix Automaton https://codeforces.com/blog/entry/20861,
KMP https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
(Only because understanding this helps understand the "links" in the Suffix
Automaton)

Note: I will constantly refer to "A short guide to suffix automata" by
"quasisphere", specifically I will use the terms r, p, q and q' to refer to the
nodes in the implementation provided.





------------ DETERMINING IF A SUBSTRING APPEARS MORE THAN ONCE -----------------

In "A short guide to suffix automata" the author "quasisphere" mentions that the
amount of times a substring appears can be obtained by running the substring on
the automaton, let's call the node it finishes in U, then obtaining the amount
of times we can reach a terminal node from U.

Something to note is that since we only care if a substring appears more than
once. We can notice that it will appear more than once if any of the following
are true:

1.- It's a terminal node but not the last node in the automaton.
2.- It has 2 or more edges.
3.- It only has 1 children and said children can reach 2 or more terminal nodes.

With that said we can simplify the part of finding all nodes that appear more
than once if we notice that every time we add a new node q' we know that node
appears more than once, that's because it's a terminal node and not the last in
the automaton.

As a note: if we append a new character C then q' won't be terminal anymore, but
it will either fall into case #2 (if C is different than the character(s) q'
already points to) or #3 (if q' already points to the same character as C, we
will create a new node add it to the automata and make q' point to it, since
that new node is terminal it can be reached 2+ times).

With that said we know that every q' added will reach 2 or more, the only case
missing is when "we don't have to split q". In that case we know that there
exists a suffix that is a prefix of the string all the way up to q, which means
q appears 2 or more times.

By handling the 2 cases mentioned above we can easily mark each node to
determine if the paths leading to it appear 2 or more times in the string.



------------ FINDING ALL SUBSTRINGS THAT APPEAR MORE THAN ONCE -----------------

The amount of distinct substrings can be obtained by doing a DFS on a node and
getting the sum of the values of all children plus 1, since we only care about
substrings that appear more than once we only add 1 if it's marked as appearing
more than once.

With that said, to do the DFS we need to already have the entire string parsed,
for the problem we need to be able to add new characters and calculate the
answer on the fly. In order to achieve this we can invert the process, by
instead of pushing the answer to the top, we can push the amount of distinct
substrings leading to a node down the automaton. The answer of all distinct
substrings would then be the sum of the values on all nodes.

To achieve that we would need to do the following:

1.- When making nodes point to r we "push" the answer from current node p.
2.- When we create a new node q' and we are making all nodes pointing to q point
to q' instead we should subtract the value of the node p from q and add it into
q' instead.

In order to account for substrings that appear more than once we can notice that
if q is not marked we know that we have 2 cases:

1.- Split q: Since substrings leading to q' appear more than once we add the
value of q' to the answer.
2.- Don't split q: Since now q appears 2 or more times we mark q and we add the
value of q to the answer.

Now we can efficiently know the answer every time we add a new character.



------------------ DELETING CHARACTERS FROM THE AUTOMATON ---------------------

This problem requires the most basic form of persistance which is undo the last
operation, there are many ways of achieving this, the most simple (yet reliable)
is to keep track of every node that got modified during a specific time, then
undoing said modification.

If in our structure we care about a value of type T instead of keeping a
variable of type T we can keep a vector of type pair<int, T>.

This vector would have all the times that the value got modified, the first part
of the pair indicates the "time" and the second the value it was set to at that
time.

Everytime we do a deleting operation remove the last value of the vector if the
time is bigger or equal to the current time. Then whenever we want to access the
value just look at the last value (since we know the time must be smaller than
current time, or equal if it just got modified at this time, which is fine).




Note: Since I based my Automaton from the guide I provided I left most of the
comments intact, added some comments of my own.

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
typedef pair<int, ll> ill;
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

#define maxSize 100001

void clearVec(vii& vec, int t) {
  while (vec.size() && vec.rbegin()->first >= t) {
    vec.pop_back();
  }
}

struct Node {
  map<char, vii> edges;
  vii link;
  vii length;
  vii original;
  vector<ill> acum;

  Node(int t) {
    link.push_back(ii(t, t == -1 ? -1 : 0));
    length.push_back(ii(t, t + 1));
    acum.push_back(ill(t, t == -1 ? 1 : 0));
    original.push_back(ill(t, 1));
  }

  Node(map<char, vii>& ed, int t, int len, int l) {
    FORC(ed, it) {
      if (it->second.size() == 0) continue;
      edges[it->first].push_back(*it->second.rbegin());
    }
    link.push_back(ii(t, l));
    length.push_back(ii(t, len));
    acum.push_back(ill(t, 0));
    original.push_back(ii(t, 0));
  }

  void addAcum(int t, ll v) {
    ll cv = acum.rbegin()->second;
    if (acum.rbegin()->first != t) {
      acum.push_back(ill(t, cv));
    }
    acum.rbegin()->second += v;
  }

  Node* clear(int t) {
    FORC(edges, it) { clearVec(it->second, t); }
    clearVec(link, t);
    clearVec(length, t);
    clearVec(original, t);
    while (acum.size() && acum.rbegin()->first >= t) {
      acum.pop_back();
    }
    return this;
  }
};

struct SuffixAutomaton {
  vector<Node*> nodes;
  vi last;
  vector<ll> ans;
  vector<map<int, Node*> > cachs;

  SuffixAutomaton() {
    // add the initial node
    Node* nn = new Node(-1);
    nodes.push_back(nn);
    last.push_back(0);
    ans.push_back(0);
  }

  Node* getNode(map<int, Node*>& cached, int id, int t) {
    if (cached.find(id) != cached.end()) return cached[id];
    return cached[id] = nodes[id];
  }

  ll removeLast() {
    int t = cachs.size() - 1;
    FORC(*cachs.rbegin(), it) { it->second->clear(t); }
    cachs.pop_back();
    last.pop_back();
    ans.pop_back();
    return *ans.rbegin();
  }

  ll addChar(char c) {
    // Persistance related stuff. Cached is used to know which nodes were
    // modified at the current time t.
    map<int, Node*> cached;
    int t = cachs.size();

    // construct r
    Node* r = new Node(t);
    nodes.push_back(r);
    int rid = nodes.size() - 1;
    cached[rid] = r;

    // add edges to r and find p with link to q
    int pid = last[t];
    Node* p = getNode(cached, pid, t);
    while (pid >= 0 &&
           (p->edges.find(c) == p->edges.end() || p->edges[c].size() == 0)) {
      // Push different ways to reach p down to r.
      r->addAcum(t, p->acum.rbegin()->second);
      p->edges[c].push_back(ii(t, rid));
      pid = p->link.rbegin()->second;
      if (pid >= 0) p = getNode(cached, pid, t);
    }
    ll tans = 0;
    if (pid != -1) {
      int qid = p->edges[c].rbegin()->second;
      Node* q = getNode(cached, qid, t);

      if (p->length.rbegin()->second + 1 == q->length.rbegin()->second) {
        // we do not have to split q, just set the correct suffix link
        r->link.rbegin()->second = qid;

        // If q is not marked we should mark it and add its value to the answer.
        if (q->original.rbegin()->second) {
          q->original.push_back(ii(t, 0));
          tans += q->acum.rbegin()->second;
        }
      } else {
        // we have to split, add q'
        Node* qq = new Node(q->edges, t, p->length.rbegin()->second + 1,
                            q->link.rbegin()->second);
        nodes.push_back(qq);
        int qqId = nodes.size() - 1;
        cached[qqId] = qq;

        // add q' as the new parent of q and r
        q->link.push_back(ii(t, qqId));
        r->link.rbegin()->second = qqId;

        // move short classes pointing to q to point to q'
        while (pid >= 0 && p->edges[c].rbegin()->second == qid) {
          p->edges[c].push_back(ii(t, qqId));

          // Push the amount of times we can visit p from q to q'.
          qq->addAcum(t, p->acum.rbegin()->second);
          q->addAcum(t, -p->acum.rbegin()->second);

          pid = p->link.rbegin()->second;
          if (pid >= 0) p = getNode(cached, pid, t);
        }

        // If q is not marked we add the value of q' to the answer.
        if (q->original.rbegin()->second) {
          tans += qq->acum.rbegin()->second;
        }
      }
    }
    cachs.push_back(cached);
    last.push_back(rid);
    ans.push_back(*ans.rbegin() + tans);
    return *ans.rbegin();
  }
};

char str[maxSize], qs[maxSize];

int main(int argc, char* argv[]) {
  _;
  while (scanf("%s", str) != EOF) {
    scanf("%s\n\n", qs);
    SuffixAutomaton s;
    int len = strlen(str);
    FOR(i, 0, len) { s.addChar(str[i]); }
    printf("%lld\n", *s.ans.rbegin());
    int len2 = strlen(qs);
    FOR(i, 0, len2) {
      if (qs[i] == '-') {
        printf("%lld\n", s.removeLast());
      } else
        printf("%lld\n", s.addChar(qs[i]));
    }
  }
  return 0;
}
