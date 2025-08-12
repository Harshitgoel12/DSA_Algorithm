📒 Notes for the Code
1. Problem Overview
You have an array of integers ar.
There are two types of updates:
Range update: For a range [l, r], divide all numbers by a prime (2, 3, or 5) a certain number of times (once per query).
Point update: Set the value at a particular index to a given number.
At the end, print the array after all queries.

2. Data Structure Used
Segment Tree with Lazy Propagation.
Instead of a single lazy value, each node has 3 lazy counters:
tree[node][0] → how many times we should divide numbers in this segment by 2.
tree[node][1] → how many times to divide by 3.
tree[node][2] → how many times to divide by 5.

3. Key Functions
updateR() — Range Update
Parameters: (node, start, end, l, r, pIdx)
Adds 1 to the lazy counter for prime factor pIdx for the segment [l, r].
Only updates the lazy array — actual division happens later in propagate().
propagate() — Push Down Lazy Updates
If internal node:
Push lazy values down to children.
Reset the lazy values at the current node.
If leaf node:
Actually divide the number in ar[start] by 2, 3, or 5 the required number of times.

updateP() — Point Update
Propagates any pending updates before changing a single index to a new value.

propagateAll()
Applies all pending lazy updates to every node in the tree before final printing.

4. Query Types
Type 1: 1 l r p
Apply range update for prime p (2, 3, or 5) on [l, r].

Type 2: 2 i d
Set ar[i] = d.

5. Final Step
After processing all queries, call propagateAll() to apply all remaining lazy updates to the array, then print.












#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> tree; // tree[node][0]=lazy count for /2, [1]=/3, [2]=/5
vector<int> ar;

// Range update: add division count for prime factor pIdx
void updateR(int node, int start, int end, int l, int r, int pIdx) {
    if (end < l || r < start) return; // No overlap
    if (l <= start && end <= r) {
        tree[node][pIdx]++;
    } else {
        int mid = (start + end) / 2;
        updateR(node * 2, start, mid, l, r, pIdx);
        updateR(node * 2 + 1, mid + 1, end, l, r, pIdx);
    }
}

// Propagate lazy updates to children or apply to leaf node
void propagate(int node, int start, int end) {
    if (start != end) {
        // Internal node → push lazy values to children
        for (int i = 0; i < 3; i++) {
            tree[node * 2][i] += tree[node][i];
            tree[node * 2 + 1][i] += tree[node][i];
            tree[node][i] = 0; // reset current lazy
        }
    } else {
        // Leaf node → actually divide the element
        while (ar[start] % 2 == 0 && tree[node][0] > 0) {
            ar[start] /= 2;
            tree[node][0]--;
        }
        while (ar[start] % 3 == 0 && tree[node][1] > 0) {
            ar[start] /= 3;
            tree[node][1]--;
        }
        while (ar[start] % 5 == 0 && tree[node][2] > 0) {
            ar[start] /= 5;
            tree[node][2]--;
        }
    }
}

// Point update: set value at index i to d
void updateP(int node, int start, int end, int i, int d) {
    propagate(node, start, end);
    if (start == end) {
        ar[start] = d;
    } else {
        int mid = (start + end) / 2;
        if (i <= mid) updateP(node * 2, start, mid, i, d);
        else updateP(node * 2 + 1, mid + 1, end, i, d);
    }
}

// Apply all pending updates to all elements
void propagateAll(int node, int start, int end) {
    propagate(node, start, end);
    if (start == end) return;
    int mid = (start + end) / 2;
    propagateAll(node * 2, start, mid);
    propagateAll(node * 2 + 1, mid + 1, end);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n; cin >> n;
    ar.resize(n + 1);
    for (int i = 1; i <= n; i++) cin >> ar[i];

    tree.assign(4 * n, vector<int>(3, 0));

    int q; cin >> q;
    while (q--) {
        int type; cin >> type;
        if (type == 1) {
            int l, r, p; cin >> l >> r >> p;
            int idx = 0;
            if (p == 3) idx = 1;
            else if (p == 5) idx = 2;
            updateR(1, 1, n, l, r, idx);
        } else {
            int i, d; cin >> i >> d;
            updateP(1, 1, n, i, d);
        }
    }

    propagateAll(1, 1, n);

    for (int i = 1; i <= n; i++) cout << ar[i] << " ";
    cout << "\n";

    return 0;
}

