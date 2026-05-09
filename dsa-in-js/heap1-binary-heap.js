// binary heap can be used to develop a priority queue that contains writes in O(log n), pop O(log n), and peek O(1)
// --> yes, peek is O(1) and pop is O(log n). Because after popping we need to rebalance the binary heap
// --> property of minHeap: parent <= children
// --> property of maxHeap: parent >= children
// --> property of generic binary heap: hasHigherPriorityFn(parent, child) === true
// Priority queue developed using an array array can reach: insert O(n), pop O(1), and peek O(1) - different usecases

class BinaryHeap {
  #items = [];
  #hasHigherPriorityFn;

  constructor({ items, hasHigherPriorityFn }) {
    if (!(items && items.length)) { throw Error("'items' field, is required") }
    // no types here in JS, so we trust hasHigherPriorityFn function and items array -> the focus is binary heap
    if (!hasHigherPriorityFn || typeof hasHigherPriorityFn !== "function") { throw Error("'hasHigherPriorityFn' function is required") }
    this.#hasHigherPriorityFn = hasHigherPriorityFn;

    this.#items = items;
    this.#buildHeapInPlace(this.#items);
  }

  #swap(addedIndex, parentIndex) {
    const temp = this.#items[addedIndex];
    this.#items[addedIndex] = this.#items[parentIndex];
    this.#items[parentIndex] = temp;
  }

  #heapifyUp({ insertedItemCurrIndex }) {
    const parentIndex = Math.floor((insertedItemCurrIndex - 1) / 2);
    if (parentIndex < 0 || insertedItemCurrIndex == parentIndex) {
      return;
    }

    const insertedItem = this.#items[insertedItemCurrIndex];
    const shouldSwap = this.#hasHigherPriorityFn(insertedItem, this.#items[parentIndex]);
    if (shouldSwap) {
      this.#swap(insertedItemCurrIndex, parentIndex);
      this.#heapifyUp({ insertedItemCurrIndex: parentIndex });
    }
  }

  #heapifyDown({ currItemPosition }) {
    let mostPriorityItemIndex = currItemPosition;
    const leftChildIndex = 2 * currItemPosition + 1;
    const rightChildIndex = 2 * currItemPosition + 2;

    const leftChildExists = leftChildIndex < this.#items.length;
    if (leftChildExists && this.#hasHigherPriorityFn(this.#items[leftChildIndex], this.#items[mostPriorityItemIndex])) {
      mostPriorityItemIndex = leftChildIndex;
    }

    const rightChildExists = rightChildIndex < this.#items.length;
    if (rightChildExists && this.#hasHigherPriorityFn(this.#items[rightChildIndex], this.#items[mostPriorityItemIndex])) {
      mostPriorityItemIndex = rightChildIndex;
    }

    // currItem = parent (start at index 0)
    // if parent already has the highest priority, do nothing
    // otherwise swap with highest priority child until heap is valid again
    if (mostPriorityItemIndex != currItemPosition) {
      this.#swap(currItemPosition, mostPriorityItemIndex);
      this.#heapifyDown({ currItemPosition: mostPriorityItemIndex });
    }
  }

  // O(n) because most heapifyDown calls run on small-height nodes.
  #buildHeapInPlace(arr) {
    const arrSize = arr.length;
    for (let i = Math.floor(arrSize / 2) - 1; i >= 0; i--) {
      this.#heapifyDown({ currItemPosition: i });
    }
  }

  // O(log n)
  push({ item }) {
    this.#items.push(item);
    if (this.#items.length >= 2) {
      this.#heapifyUp({ insertedItemCurrIndex: this.#items.length - 1 });
    }
  }

  // O(log n)
  pop() {
    const currLength = this.#items.length;
    if (currLength == 0) { return null; }

    const itemToReturn = this.#items[0];
    this.#items[0] = this.#items[currLength - 1]; // last item is new root
    this.#items.pop(); // remove last item from array

    if (currLength > 1) {
      this.#heapifyDown({ currItemPosition: 0 }); // fix top down
    }

    return itemToReturn;
  }

  // O(1)
  peek() {
    if (!this.#items.length) { return null; }
    return this.#items[0];
  }

  printItems() {
    console.log('this.#items', this.#items);
  }

  consumeAll(callback) {
    console.log(this.constructor.name, 'consumeAll:')
    console.log('this.#hasHigherPriorityFn.toString()', this.#hasHigherPriorityFn.toString())
    while (this.#items.length) {
      callback(this.pop());
    }
  }
}

// smallest item at root index = 0
class MinHeap extends BinaryHeap {
  constructor({ items }) {
    super({ items, hasHigherPriorityFn: (itemA, itemB) => itemA < itemB });
  }
}

// biggest item at root index = 0
class MaxHeap extends BinaryHeap {
  constructor({ items }) {
    super({ items, hasHigherPriorityFn: (itemA, itemB) => itemA > itemB });
  }
}

console.log('--------------------------------------------------------')

const minHeapTest = new MinHeap({ items: [8, 7, 6, 5, 4, 3, 2, 1] });
minHeapTest.printItems();
minHeapTest.consumeAll(console.log)

console.log('--------------------------------------------------------')

const maxHeap = new MaxHeap({ items: [1, 2, 3, 4, 5, 6, 7, 8] });
maxHeap.printItems();
maxHeap.consumeAll(console.log)

console.log('--------------------------------------------------------')

const people = [{ name: "João", age: 22 }, { name: "Turing", age: 41 }, { name: "Ada Lovelace", age: 36 }]
const elderlyPeopleFirstHeap = new BinaryHeap({ items: people, hasHigherPriorityFn: (a, b) => a.age > b.age });
elderlyPeopleFirstHeap.consumeAll(console.log)

console.log('--------------------------------------------------------')