# João Godinho Jardim M2

# 23. Merge k Sorted Lists

- https://leetcode.com/problems/merge-k-sorted-lists/description/

You are given an array of k linked-lists lists, each linked-list is sorted in ascending order.

Merge all the linked-lists into one sorted linked-list and return it.

Example 1:

Input: lists = [[1,4,5],[1,3,4],[2,6]]
Output: [1,1,2,3,4,4,5,6]
Explanation: The linked-lists are:
[
1->4->5,
1->3->4,
2->6
]
merging them into one sorted linked list:
1->1->2->3->4->4->5->6

Example 2:
Input: lists = []
Output: []

Example 3:
Input: lists = [[]]
Output: []

# Run with valgrind: (Dr memory and valgrind are incompatible with macos)

1 - install docker on oficial website
2 - Run on terminal: `docker build -t valgrind-runner . && docker run valgrind-runner`

# My solution Big O:
// k = amount of lists
// n = each list item

- TIME: O(n*k)
- SPACE: O(n)
