export class Stack {
  constructor() {
    this.items = [];
  }

  insert(val) {
    this.items.push(val);
  }

  pop() {
    return this.items.pop() ?? -1; // -1 means empty
  }
}

const stack = new Stack();

stack.insert(10)
stack.insert(12)
stack.insert(13)
console.log('stack.pop()', stack.pop())
console.log('stack.pop()', stack.pop())
console.log('stack.pop()', stack.pop())
console.log('stack.pop()', stack.pop())