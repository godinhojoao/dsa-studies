
export class SinglyLinkedListNode {
  constructor(val) {
    this.next = null;
    this.val = val ?? -1;
  }

  static insert(head, val) {
    let prev = head;
    let currNode = head;
    while (currNode != null) {
      prev = currNode;
      currNode = currNode.next;
    }
    prev.next = new SinglyLinkedListNode(val);
  }

  static search(head, val) {
    let currNode = head;
    while (currNode != null) {
      if (currNode.val === val) { return currNode; }
      currNode = currNode.next;
    }

    return null; // not found
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
    while(curr!=null) {
      let tempNext = curr.next;
      curr.next = prev;
      prev = curr;
      curr = tempNext;
    }
    return prev;
  }

}

const head = new SinglyLinkedListNode(3)
SinglyLinkedListNode.insert(head, 5);
SinglyLinkedListNode.insert(head, 10);

console.log('SinglyLinkedListNode.search(head, 3)', SinglyLinkedListNode.search(head, 3))
console.log('SinglyLinkedListNode.search(head, 999)', SinglyLinkedListNode.search(head, 999))
SinglyLinkedListNode.traverse(head);
const reversedHead = SinglyLinkedListNode.reverse(head);
SinglyLinkedListNode.traverse(reversedHead);


