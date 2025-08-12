Notes — Lazy Propagation (Segment Tree)
Problem: We need to do two things efficiently:

Update a range (add a value to every element in [L, R])

Query a range (get the sum of elements in [L, R])

Why Normal Approach Fails:

Without lazy propagation, a range update takes O(n) in worst case.

With Segment Tree only (without lazy), we can do it in O(log n) but only for point updates.
Range updates still take longer.

Lazy Propagation Idea:

Store pending updates in a separate array lazy[].

When visiting a node:

If it has a pending value → apply it to the node.

Push that pending value down to its children (so they get updated later only if needed).

This way, we don’t repeatedly update every element — only when required.

C++ Code
cpp
Copy
Edit
#include <bits/stdc++.h>
using namespace std;

vector<long long> seg;  // Segment tree for storing sums
vector<long long> lazy; // Lazy array for storing pending updates

// Build the segment tree
void build(vector<int> &arr, int idx, int start, int end) {
    if (start == end) {
        seg[idx] = arr[start]; // Leaf node = array value
        return;
    }
    int mid = (start + end) / 2;
    build(arr, idx * 2, start, mid);
    build(arr, idx * 2 + 1, mid + 1, end);
    seg[idx] = seg[idx * 2] + seg[idx * 2 + 1];
}

// Propagate pending updates to children
void propagate(int idx, int start, int end) {
    if (lazy[idx] != 0) {
        // Apply pending update to this node
        seg[idx] += (end - start + 1) * lazy[idx];

        // If not a leaf node, mark children as lazy
        if (start != end) {
            lazy[idx * 2] += lazy[idx];
            lazy[idx * 2 + 1] += lazy[idx];
        }

        // Clear lazy value for this node
        lazy[idx] = 0;
    }
}

// Range update: Add `val` to all elements in [l, r]
void update(int idx, int start, int end, int l, int r, int val) {
    propagate(idx, start, end); // Apply any pending updates first

    // No overlap
    if (start > r || end < l) return;

    // Total overlap
    if (l <= start && end <= r) {
        lazy[idx] += val;
        propagate(idx, start, end);
        return;
    }

    // Partial overlap
    int mid = (start + end) / 2;
    update(idx * 2, start, mid, l, r, val);
    update(idx * 2 + 1, mid + 1, end, l, r, val);
    seg[idx] = seg[idx * 2] + seg[idx * 2 + 1];
}

// Range query: Sum of elements in [l, r]
long long query(int idx, int start, int end, int l, int r) {
    propagate(idx, start, end); // Apply pending updates first

    // No overlap
    if (start > r || end < l) return 0;

    // Total overlap
    if (l <= start && end <= r) return seg[idx];

    // Partial overlap
    int mid = (start + end) / 2;
    return query(idx * 2, start, mid, l, r) +
           query(idx * 2 + 1, mid + 1, end, l, r);
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    int n = arr.size();

    seg.resize(4 * n, 0);
    lazy.resize(4 * n, 0);

    build(arr, 1, 0, n - 1);

    cout << "Initial sum of [1,3]: " << query(1, 0, n - 1, 1, 3) << "\n";

    update(1, 0, n - 1, 1, 3, 2); // Add 2 to elements from index 1 to 3
    cout << "After adding 2 to [1,3], sum of [1,3]: "
         << query(1, 0, n - 1, 1, 3) << "\n";

    update(1, 0, n - 1, 0, 4, 1); // Add 1 to whole array
    cout << "After adding 1 to all, sum of [0,4]: "
         << query(1, 0, n - 1, 0, 4) << "\n";

    return 0;
}


// How to Remember
// Always propagate before doing anything in update/query.

// Lazy value means “I have to update my children later”.

// In propagate():

// Apply pending update to current node.

// Push it to children if not leaf.

// Clear own lazy value.

// update() and query() are almost the same — only difference is that update changes lazy[].

