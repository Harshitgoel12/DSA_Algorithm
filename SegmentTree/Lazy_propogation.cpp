// suppose you are given an array   in which we have to make changes in particular range
// suppose the array is  [4,3 ,9,3,9,8,7,2]  and the query are
// 1 4 6 1  means  i have to increase the  value from index 4 to index 6 by 1
// 0 4 if it is given we have to find in particular index what value is present 


//to solve this type of problem we create a dummy segmentTree
// when query happend if parent node has value for updation and we go in depth then we simple make zero at parent 
// node in dummy segmentTree and in the bottom dummy node in segmentTree we add the parent dummy segment values


//so what happend in range update  there are 3 cases

// case 1: if the range is out of bound means we have to increment the value in range [3,5] but we are at
//   [1,2] or [6,8] so we simple return from here

//   case 2 : if fully inside the range then we do increase the value of parentnode and increase the value of left 
//             and right child in dummy segment tree when we go down we pass throw the bottom nodes and make the parent
//          node value in dummy segment tree to zero

// case 3 : if partially is in the range then we call for left and right subtree 



  // -----------------------------------------------------------------> Code <----------------------------------------------------------------------------



  
#include <bits/stdc++.h>
using namespace std;

/*
  Segment Tree with Lazy Propagation
  Supports:
    1. Range Update (add value to all elements in range)
    2. Point Query (get value at specific index)
*/

struct SegmentTree {
    vector<int> lazy; // stores lazy updates
    vector<int> ar;   // original array

    // Constructor to initialize the tree
    SegmentTree(vector<int> &arr) {
        ar = arr;
        lazy.assign(4 * arr.size(), 0); // 4*n size is safe for segment tree
        build(1, 0, ar.size() - 1);
    }

    // Build function (initializes the tree)
    void build(int node, int start, int end) {
        if (start == end) {
            lazy[node] = ar[start]; // leaf node stores array value
        } else {
            int mid = (start + end) / 2;
            build(node * 2, start, mid);
            build(node * 2 + 1, mid + 1, end);
            lazy[node] = 0; // initially, no pending update at internal nodes
        }
    }

    // Propagate lazy updates to child nodes
    void propagate(int node, int start, int end) {
        if (start == end) return; // leaf node, no propagation needed
        int left = node * 2;
        int right = node * 2 + 1;

        lazy[left] += lazy[node];
        lazy[right] += lazy[node];
        lazy[node] = 0; // reset after propagation
    }

    // Range update: add `val` to all elements in range [l, r]
    void update(int node, int start, int end, int l, int r, int val) {
        if (end < l || r < start) return; // completely outside range

        if (l <= start && end <= r) { // completely inside range
            lazy[node] += val;
            return;
        }

        // partial overlap → propagate and update children
        propagate(node, start, end);
        int mid = (start + end) / 2;
        update(node * 2, start, mid, l, r, val);
        update(node * 2 + 1, mid + 1, end, l, r, val);
    }

    // Point query: get value at index `idx`
    int query(int node, int start, int end, int idx) {
        if (start == end) {
            return lazy[node]; // value at leaf
        }
        propagate(node, start, end); // push updates down before going deeper
        int mid = (start + end) / 2;
        if (idx <= mid)
            return query(node * 2, start, mid, idx);
        else
            return query(node * 2 + 1, mid + 1, end, idx);
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};
    SegmentTree st(arr);

    // Example: Add +10 to range [1, 3]
    st.update(1, 0, arr.size() - 1, 1, 3, 10);

    // Query: Get value at index 2
    cout << "Value at index 2: " << st.query(1, 0, arr.size() - 1, 2) << "\n";

    // Query: Get value at index 4 (no update here)
    cout << "Value at index 4: " << st.query(1, 0, arr.size() - 1, 4) << "\n";

    return 0;
}
