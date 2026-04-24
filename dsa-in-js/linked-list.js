
export class SinglyLinkedListNode {
  constructor(val) {
    this.next = null;
    this.val = val ?? -1;
  }

  // time O(n) but if we could save tail so it would be O(1)
  static insert(head, val) {
    let prev = head;
    let currNode = head;
    while (currNode != null) {
      prev = currNode;
      currNode = currNode.next;
    }
    prev.next = new SinglyLinkedListNode(val);
    return prev.next;
  }

  static search(head, val) {
    let currNode = head;
    while (currNode != null) {
      if (currNode.val === val) { return currNode; }
      currNode = currNode.next;
    }

    return null; // not found
  }

  static delete(head, val) {
    if (head.val === val) {
      return head.next;
    }

    let currNode = head;
    let nextNode = head.next;

    while (currNode.next != null) {
      if (nextNode.val === val) {
        currNode.next = nextNode.next;
        return head;
      }
      currNode = nextNode;
      nextNode = nextNode.next;
    }

    return head; // not found case
  }

  static traverse(head) {
    console.log('traverse:')
    let currNode = head;
    while (currNode != null) {
      console.log('currNode.val', currNode.val);
      currNode = currNode.next;
    }
  }

  // time O(n), space O(1)
  static reverse(head) {
    let prev = null;
    let curr = head;
    // 3 -> 5 -> 10 -> null
    // 10 -> 5 -> 3 -> null
    while (curr != null) {
      let tempNext = curr.next;
      curr.next = prev;
      prev = curr;
      curr = tempNext;
    }
    return prev;
  }
}

// let head = new SinglyLinkedListNode(3)
// SinglyLinkedListNode.insert(head, 5);
// SinglyLinkedListNode.insert(head, 10);

// console.log('SinglyLinkedListNode.search(head, 3)', SinglyLinkedListNode.search(head, 3))
// console.log('SinglyLinkedListNode.search(head, 999)', SinglyLinkedListNode.search(head, 999))
// SinglyLinkedListNode.traverse(head);

// head = SinglyLinkedListNode.reverse(head);
// SinglyLinkedListNode.traverse(head);

// head = SinglyLinkedListNode.delete(head, 10);
// SinglyLinkedListNode.traverse(head);
