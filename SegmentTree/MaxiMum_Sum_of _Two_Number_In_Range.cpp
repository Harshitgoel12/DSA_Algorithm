you have given an array and a number of queries their are two types of query first is 
you have to find the sum of two maximum number in particular range and in second one you have to update the value 
of particular index


solution -> their are 3 type of situations 
   case 1 : both are on left side
   case 2: both are on the right side
   case 3 : one is on the left and other is on the right side


in this problem we return two things to the parent node one is the sum of left and right part if on the below level
their is an leaf node if no leaf node then the max of left and write sum values and other is the
maximum value of both of the node

that means The sum of the left and right parts – if at the lower level there is a leaf node. 
If there is no leaf node, then we return the maximum of the left and right sum values instead.
The maximum value of both child nodes.


Problem Summary
You need to support two operations efficiently on an array:

Range Query: Find the sum of the two largest numbers within a given range.

Point Update: Update the value at a specific index.

Segment Tree Structure
Each node in the segment tree contains:

max1: The largest value in that segment.

max2: The second largest value in that segment.

Note: If the segment has only one element, max2 is considered as negative infinity (or a sentinel like INT_MIN), meaning it doesn't exist.

Merging Logic
To combine information from two child nodes (left, right) into the parent node:

Consider these four values: left.max1, left.max2, right.max1, right.max2.

Pick the top two values out of these as:

New max1 = highest among the four.

New max2 = second highest among the four.

This logic naturally covers all 3 situations:

Both largest values in the left segment.

Both in the right segment.

One in each segment.

Operations Supported
Build (O(n)):

Leaf node: max1 = arr[i], max2 = INT_MIN.

Internal node: Merge its two children.

Update (O(log n)):

Change the array value at an index.

Recompute that leaf’s node, then merge upward.

Query (O(log n)):

Get a node that represents the range.

Sum returned node’s max1 + max2 for the answer.

Best Practices
Always initialize max2 as INT_MIN at leaf, in case segment size is 1.

When merging, sorting the four candidates is simple and efficient for just two picks.

Keep your tree size at least 4 * n so there’s enough room for nodes.




#include <bits/stdc++.h>
using namespace std;

// ---------------------------
// Struct to store information in each segment tree node
// ---------------------------
// sum → sum of two largest numbers in the range
// max → largest number in the range
struct Pair {
    int sum;
    int max;
    Pair(int s = 0, int m = 0) {
        sum = s;
        max = m;
    }
};

// Global variables for array and segment tree
vector<Pair> tree; // segment tree
vector<int> arr;   // input array

// ---------------------------
// Function to combine results from left & right children
// ---------------------------
// Logic:
//  - Case 1: both largest numbers are in left child → left.sum
//  - Case 2: both largest numbers are in right child → right.sum
//  - Case 3: one from left and one from right → left.max + right.max
// The max value of the combined range is max(left.max, right.max)
Pair calc(Pair left, Pair right) {
    Pair ans;
    // Take the best sum from either side
    ans.sum = max(left.sum, right.sum);       // Case 1 & Case 2
    ans.sum = max(ans.sum, left.max + right.max); // Case 3
    // Max value of the whole range
    ans.max = max(left.max, right.max);
    return ans;
}

// ---------------------------
// Build the segment tree
// ---------------------------
void build(int node, int start, int end) {
    if (start == end) {
        // Leaf node: sum and max are the same (only one element)
        tree[node] = Pair(arr[start], arr[start]);
    } else {
        int mid = (start + end) / 2;
        // Build left child
        build(node * 2, start, mid);
        // Build right child
        build(node * 2 + 1, mid + 1, end);
        // Merge results
        tree[node] = calc(tree[node * 2], tree[node * 2 + 1]);
    }
}

// ---------------------------
// Update the value at index idx to val
// ---------------------------
void update(int node, int start, int end, int idx, int val) {
    if (start == end) {
        // Update the leaf node
        arr[start] = val;
        tree[node] = Pair(val, val);
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update(node * 2, start, mid, idx, val);
        } else {
            update(node * 2 + 1, mid + 1, end, idx, val);
        }
        // Recalculate current node after update
        tree[node] = calc(tree[node * 2], tree[node * 2 + 1]);
    }
}

// ---------------------------
// Query the sum of two largest numbers in range [l, r]
// ---------------------------
Pair query(int node, int start, int end, int l, int r) {
    if (end < l || start > r) {
        // Out of range → return neutral values
        return Pair(0, 0);
    }
    if (l <= start && end <= r) {
        // Fully inside range
        return tree[node];
    }
    int mid = (start + end) / 2;
    // Query both halves
    Pair left = query(node * 2, start, mid, l, r);
    Pair right = query(node * 2 + 1, mid + 1, end, l, r);
    // Merge results
    return calc(left, right);
}

// ---------------------------
// Main function
// ---------------------------
int main() {
    int n;
    cin >> n;
    arr.resize(n);
    tree.resize(4 * n); // Segment tree size ~ 4n

    // Read array
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Build tree
    build(1, 0, n - 1);

    int q;
    cin >> q; // number of queries
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            // Update query
            int idx, val;
            cin >> idx >> val;
            update(1, 0, n - 1, idx, val);
        } else {
            // Range query
            int l, r;
            cin >> l >> r;
            Pair ans = query(1, 0, n - 1, l, r);
            cout << ans.sum << "\n";
        }
    }

    return 0;
}




