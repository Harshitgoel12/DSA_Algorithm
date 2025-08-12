you are given an array  of integer and their are two type of queries
one is  [type,start,end] here type is 0 and we have to find the sum  of square of every element
from start to end
another is [type ,start ,end ,val] here type is 1 and we have to increase the value by val


#include <bits/stdc++.h>
using namespace std;

struct PairVal {
    long long sum;    // sum of elements in the segment
    long long sumSq;  // sum of squares of elements in the segment
    PairVal(long long s = 0, long long sq = 0) {
        sum = s;
        sumSq = sq;
    }
};

class SegmentTree {
private:
    vector<long long> arr;      // input array
    vector<PairVal> tree;       // segment tree
    vector<long long> lazy;     // lazy array for range updates
    int n;                      // size of input array

    // Helper function to merge two nodes
    PairVal combine(const PairVal &left, const PairVal &right) {
        return PairVal(left.sum + right.sum, left.sumSq + right.sumSq);
    }

    // Build the tree
    void build(int node, int start, int end) {
        if (start == end) {
            tree[node] = PairVal(arr[start], arr[start] * arr[start]);
        } else {
            int mid = (start + end) / 2;
            build(node * 2, start, mid);
            build(node * 2 + 1, mid + 1, end);
            tree[node] = combine(tree[node * 2], tree[node * 2 + 1]);
        }
    }

    // Propagate lazy updates to children
    void propagate(int node, int start, int end) {
        if (lazy[node] != 0) {
            long long val = lazy[node];
            long long count = end - start + 1;

            // Update sumSq using formula:
            // (a + val)^2 = a^2 + 2*a*val + val^2
            tree[node].sumSq += count * val * val + 2 * val * tree[node].sum;
            tree[node].sum += count * val;

            // Push to children if not leaf
            if (start != end) {
                lazy[node * 2] += val;
                lazy[node * 2 + 1] += val;
            }
            lazy[node] = 0; // clear lazy value after applying
        }
    }

    // Range update
    void update(int node, int start, int end, int l, int r, int val) {
        propagate(node, start, end);

        if (end < l || start > r) return; // completely outside range

        if (l <= start && end <= r) { // completely inside range
            lazy[node] += val;
            propagate(node, start, end);
            return;
        }

        // Partially inside range
        int mid = (start + end) / 2;
        update(node * 2, start, mid, l, r, val);
        update(node * 2 + 1, mid + 1, end, l, r, val);
        tree[node] = combine(tree[node * 2], tree[node * 2 + 1]);
    }

    // Range query
    PairVal query(int node, int start, int end, int l, int r) {
        if (end < l || start > r) return PairVal(0, 0); // no overlap

        propagate(node, start, end);

        if (l <= start && end <= r) return tree[node]; // fully inside range

        // partial overlap
        int mid = (start + end) / 2;
        PairVal left = query(node * 2, start, mid, l, r);
        PairVal right = query(node * 2 + 1, mid + 1, end, l, r);
        return combine(left, right);
    }

public:
    // Constructor
    SegmentTree(vector<long long> &input) {
        arr = input;
        n = arr.size();
        tree.assign(n * 4, PairVal());
        lazy.assign(n * 4, 0);
        build(1, 0, n - 1);
    }

    // Public update function
    void update(int l, int r, int val) {
        update(1, 0, n - 1, l, r, val);
    }

    // Public query function (returns sum of squares in range)
    long long querySumSq(int l, int r) {
        return query(1, 0, n - 1, l, r).sumSq;
    }
};

int main() {
    vector<long long> arr = {1, 2, 3, 4, 5};
    SegmentTree st(arr);

    cout << "Initial sum of squares (0..4): " << st.querySumSq(0, 4) << "\n";

    st.update(1, 3, 2); // add 2 to range 1..3

    cout << "After update sum of squares (0..4): " << st.querySumSq(0, 4) << "\n";
    cout << "Query sum of squares (2..4): " << st.querySumSq(2, 4) << "\n";

    return 0;
}


🔹 Notes & Explanation:
PairVal struct

Stores sum and sumSq for each segment.

Lazy Propagation

lazy[node] stores pending increment for a segment.

When we visit that node, we first apply it (propagate), then push to children.

Update formula for sum of squares:

kotlin
Copy
Edit
sumSq += count * val^2 + 2 * val * sum
sum   += count * val
Time Complexity

Update: O(log n)

Query: O(log n)





