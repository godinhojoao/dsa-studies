import { DoublyLinkedListNode } from './doubly-linked-list.js'

export class Deque {
  #itemsHead;
  #itemsTail;
  #size;

  constructor(initialValues) {
    this.#itemsHead = null;
    this.#itemsTail = null;
    this.#size = 0;
    for (let i = 0; i < initialValues.length; i++) { // O(n)
      this.pushEnd(initialValues[i]);
    }
  }

  // O(1)
  isEmpty() {
    return this.#size === 0;
  }

  // O(1)
  size() {
    return this.#size;
  }

  // O(1)
  pushEnd(val) {
    if (this.isEmpty()) {
      this.#itemsHead = new DoublyLinkedListNode({ val });
      this.#itemsTail = this.#itemsHead;
    } else {
      this.#itemsTail = DoublyLinkedListNode.insert(this.#itemsTail, val);
    }
    this.#size++;
  }

  // O(1)
  pushFront(val) {
    if (this.isEmpty()) {
      this.#itemsHead = new DoublyLinkedListNode({ val });
      this.#itemsTail = this.#itemsHead;
    } else {
      const newNode = new DoublyLinkedListNode({ val, next: this.#itemsHead })
      this.#itemsHead.prev = newNode;
      this.#itemsHead = newNode;
    }
    this.#size++;
  }

  // O(1)
  popFront() {
    if (this.isEmpty()) return -1;

    const tempHeadVal = this.#itemsHead.val;
    const result = DoublyLinkedListNode.deleteHead({ head: this.#itemsHead, tail: this.#itemsTail });
    this.#itemsHead = result.head;
    this.#itemsTail = result.tail;
    this.#size--;
    return tempHeadVal;
  }

  // O(1)
  popBack() {
    if (this.isEmpty() || this.#itemsTail === null) return -1;

    const tempTailVal = this.#itemsTail.val;
    const result = DoublyLinkedListNode.deleteTail({ head: this.#itemsHead, tail: this.#itemsTail });
    this.#itemsHead = result.head;
    this.#itemsTail = result.tail;
    this.#size--;
    return tempTailVal;
  }

  // O(1)
  peekFront() {
    if (this.isEmpty()) return -1;
    return this.#itemsHead.val;
  }

  // O(1)
  peekBack() {
    if (this.isEmpty() || this.#itemsTail === null) return -1;
    return this.#itemsTail.val;
  }

  consumeBack(processFunc) {
    while (!this.isEmpty()) {
      const val = this.popBack();
      processFunc(val);
    }
  }

  consumeFront(processFunc) {
    while (!this.isEmpty()) {
      const val = this.popFront();
      processFunc(val);
    }
  }
}

// const deque = new Deque([10, 0])
// console.log('initial size:', deque.size());
// console.log('peekFront:', deque.peekFront());
// console.log('peekBack:', deque.peekBack());

// deque.pushFront(5);
// console.log('after pushFront(5)');
// console.log('peekFront:', deque.peekFront());
// console.log('peekBack:', deque.peekBack());
// console.log('size:', deque.size());

// deque.pushEnd(20);
// console.log('after pushEnd(20)');
// console.log('peekFront:', deque.peekFront());
// console.log('peekBack:', deque.peekBack());
// console.log('size:', deque.size());

// console.log('popFront:', deque.popFront());
// console.log('after popFront');
// console.log('peekFront:', deque.peekFront());
// console.log('peekBack:', deque.peekBack());
// console.log('size:', deque.size());

// console.log('popBack:', deque.popBack());
// console.log('after popBack');
// console.log('peekFront:', deque.peekFront());
// console.log('peekBack:', deque.peekBack());
// console.log('size:', deque.size());

// console.log('popFront:', deque.popFront());
// console.log('popFront:', deque.popFront());
// console.log('popFront (empty):', deque.popFront());
// console.log('isEmpty:', deque.isEmpty());
// console.log('size:', deque.size());

// deque.pushEnd(10)
// deque.pushEnd(20)
// deque.pushEnd(30)
// console.log('size:', deque.size()); // 3, since we have [10,20,30]
// deque.consumeBack((val) => console.log(`val:${val}`))