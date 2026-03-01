export class Stack {
  #items; // private field

  constructor() {
    this.#items = [];
  }

  insert(val) {
    this.#items.push(val);
  }

  pop() {
    return this.#items.pop() ?? -1; // -1 means empty
  }

  peek() {
    return this.#items[this.#items.length - 1] ?? -1;
  }

  size() {
    return this.#items.length;
  }
}

const stack = new Stack();
stack.insert(10)
stack.insert(12)
stack.insert(13)
console.log('stack.size()', stack.size())
console.log('stack.peek()', stack.peek())
console.log('stack.pop()', stack.pop())
console.log('stack.pop()', stack.pop())
console.log('stack.pop()', stack.pop())
console.log('stack.pop()', stack.pop())
console.log('stack.peek()', stack.peek())
console.log('stack.size()', stack.size())