
// not circular (head doesn't contain prev == tail)
export class DoublyLinkedListNode {
  constructor({ val, next, prev }) {
    this.next = next ?? null;
    this.prev = prev ?? null;
    this.val = val ?? -1;
  }

  // time O(1)
  static insert(node, val) {
    if (!node) return new DoublyLinkedListNode({ val }); // empty list case

    const newNode = new DoublyLinkedListNode({ val, prev: node, next: node.next });
    if (node.next != null) {
      node.next.prev = newNode;
    }
    node.next = newNode;
    return newNode;
  }

  // O(n)
  static search(head, val) {
    let currNode = head;
    while (currNode != null) {
      if (currNode.val === val) { return currNode; }
      currNode = currNode.next;
    }

    return null; // not found
  }

  // O(n)
  static deleteByVal(head, val) {
    if (head == null) return null;

    let currNode = head;
    while (currNode != null) {
      if (currNode.val === val) {
        // here we have to make this deleted node unreachable so gc cleans it
        const isTailDeleted = currNode.next === null;

        if (currNode.prev) {
          currNode.prev.next = currNode.next;
        } else {
          head = currNode.next;
        }
        if (currNode.next) {
          currNode.next.prev = currNode.prev;
        }


        return { head, newTail: isTailDeleted ? currNode.prev : undefined };
      }
      currNode = currNode.next;
    }

    return { head, newTail: undefined }; // not found case
  }

  // O(1)
  static deleteHead({ head, tail }) {
    if (!head) return { head: null, tail };

    const newHead = head.next;
    if (newHead) {
      newHead.prev = null;
    } else {// old head doesn't contain next, so it was tail, and now both are null (list empty)
      tail = null;
    }
    return { head: newHead, tail };
  }

  // O(1)
  static deleteTail({ head, tail }) {
    if (!tail) return { head, tail: null };
    const newTail = tail.prev;
    if (newTail) {
      newTail.next = null;
    } else {
      head = null; // old tail doesnt contain prev, so it was head,  and now both are null (list empty)
    }
    return { head, tail: newTail };
  }

  // O(n)
  static traverseForward(head) {
    console.log('traverseForward:')
    let currNode = head;
    while (currNode != null) {
      console.log('currNode.val', currNode.val);
      currNode = currNode.next;
    }
  }

  // O(n)
  static traverseBackward(tail) {
    console.log('traverseBackward:')
    let currNode = tail;
    while (currNode != null) {
      console.log('currNode.val', currNode.val);
      currNode = currNode.prev;
    }
  }

  // time O(n), space O(1)
  static reverse(head) {
    if (head == null) return null;

    let currNode = head;
    let newHead = null;

    while (currNode !== null) {
      const tempNext = currNode.next;

      currNode.next = currNode.prev;
      currNode.prev = tempNext;

      newHead = currNode;
      currNode = tempNext;
    }

    return { head: newHead, tail: head };
  }
}

// let head = new DoublyLinkedListNode({ val: 3 });
// let tail = DoublyLinkedListNode.insert(head, 5);
// tail = DoublyLinkedListNode.insert(tail, 10);

// console.log('initial');
// DoublyLinkedListNode.traverseForward(head);
// DoublyLinkedListNode.traverseBackward(tail);

// console.log('search 5:', DoublyLinkedListNode.search(head, 5));
// console.log('search 99:', DoublyLinkedListNode.search(head, 99));

// ({ head, tail } = DoublyLinkedListNode.reverse(head));
// console.log('after reverse');
// DoublyLinkedListNode.traverseForward(head);
// DoublyLinkedListNode.traverseBackward(tail);

// let res = DoublyLinkedListNode.deleteByVal(head, 5);
// head = res.head;
// if (res.newTail !== undefined) tail = res.newTail;
// console.log('after delete 5');
// DoublyLinkedListNode.traverseForward(head);
// DoublyLinkedListNode.traverseBackward(tail);

// ({ head, tail } = DoublyLinkedListNode.deleteHead({ head, tail }));
// console.log('after deleteHead');
// DoublyLinkedListNode.traverseForward(head);
// DoublyLinkedListNode.traverseBackward(tail);

// ({ head, tail } = DoublyLinkedListNode.deleteTail({ head, tail }));
// console.log('after deleteTail');
// DoublyLinkedListNode.traverseForward(head);
// DoublyLinkedListNode.traverseBackward(tail);