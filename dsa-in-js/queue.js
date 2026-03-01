class QueueNode {
  constructor(val) {
    this.val = val;
    this.next = null;
  }
}

export class Queue {
  constructor() {
    this.head = null;
    this.tail = null;
  }

  insert(val) {
    if (this.head == null) {
      this.head = new QueueNode(val);
      this.tail = this.head;
    } else {
      const tempTail = this.tail;
      this.tail = new QueueNode(val);
      tempTail.next = this.tail;
    }
  }

  dequeue() {
    if (this.head == null) {
      return -1;
    }
    const tempHead = this.head;
    this.head = this.head.next;
    return tempHead.val;
  }
}

const queue = new Queue();

queue.insert(10)
queue.insert(12)
queue.insert(13)
console.log('queue.dequeue()', queue.dequeue()) // 10
console.log('queue.dequeue()', queue.dequeue()) // 12
console.log('queue.dequeue()', queue.dequeue()) // 13
console.log('queue.dequeue()', queue.dequeue()) // -1