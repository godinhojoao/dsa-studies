// https://leetcode.com/problems/reverse-linked-list/

/**
 * Definition for singly-linked list.
 * function ListNode(val, next) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.next = (next===undefined ? null : next)
 * }
 */
/**
 * @param {ListNode} head
 * @return {ListNode}
 */
// time O(n), space O(1)
var reverseList = function (head) {
  if (!(head && head.next)) { return head };

  let prev = null;
  let currNode = head;

  while (currNode != null) {
    let tempNext = currNode.next;
    currNode.next = prev;

    prev = currNode;
    currNode = tempNext;
  }

  return prev; // as currnode == null, prev is the old tail, that now is the head
};