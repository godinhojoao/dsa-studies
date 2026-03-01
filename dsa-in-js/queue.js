class QueueNode {
  constructor(val) {
    this.val = val;
    this.next = null;
  }
}

export class Queue {
  #head;
  #tail;
  #size;
  constructor() {
    this.#head = null;
    this.#tail = null;
    this.#size = 0;
  }

  insert(val) {
    if (this.#head == null) {
      this.#head = new QueueNode(val);
      this.#tail = this.#head;
    } else {
      const tempTail = this.#tail;
      this.#tail = new QueueNode(val);
      tempTail.next = this.#tail;
    }
    this.#size++;
  }

  dequeue() {
    if (this.#head == null) {
      return -1;
    }
    const tempHead = this.#head;
    this.#head = this.#head.next;
    this.#size--;
    return tempHead.val;
  }

  front() {
    return this.#head ? this.#head.val : -1;
  }

  size() {
    return this.#size;
  }
}

const queue = new Queue();

queue.insert(10)
queue.insert(12)
queue.insert(13)
console.log('queue.size()', queue.size()) // 3
console.log('queue.front()', queue.front()) // 10
console.log('queue.dequeue()', queue.dequeue()) // 10
console.log('queue.dequeue()', queue.dequeue()) // 12
console.log('queue.dequeue()', queue.dequeue()) // 13
console.log('queue.dequeue()', queue.dequeue()) // -1
console.log('queue.front()', queue.front()) // -1
console.log('queue.size()', queue.size()) // 0