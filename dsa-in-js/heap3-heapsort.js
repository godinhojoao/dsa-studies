function swap(items, addedIndex, parentIndex) {
  const temp = items[addedIndex];
  items[addedIndex] = items[parentIndex];
  items[parentIndex] = temp;
}

function heapifyDown({ currItemPosition, items, lastHeapIndex }) {
  let mostPriorityItemIndex = currItemPosition;
  const leftChildIndex = 2 * currItemPosition + 1;
  const rightChildIndex = 2 * currItemPosition + 2;

  const leftChildExists = leftChildIndex < items.length && leftChildIndex < lastHeapIndex;
  if (leftChildExists && items[leftChildIndex] > items[mostPriorityItemIndex]) {
    mostPriorityItemIndex = leftChildIndex;
  }

  const rightChildExists = rightChildIndex < items.length && rightChildIndex < lastHeapIndex;
  if (rightChildExists && items[rightChildIndex] > items[mostPriorityItemIndex]) {
    mostPriorityItemIndex = rightChildIndex;
  }

  // if parent already has the highest priority, do nothing
  if (mostPriorityItemIndex != currItemPosition) {
    swap(items, currItemPosition, mostPriorityItemIndex);
    heapifyDown({ items, currItemPosition: mostPriorityItemIndex, lastHeapIndex });
  }
}

function buildHeapInPlace(arr) {
  const arrSize = arr.length;
  for (let i = Math.floor(arrSize / 2) - 1; i >= 0; i--) {
    heapifyDown({ items: arr, currItemPosition: i, lastHeapIndex: arrSize }); // at start all array is a heap so lastHeapIndex = arrsize
  }
}

/*
1. Build a MaxHeap in `O(n)` in place using an array.
    1. The **heap area** starts from `0` to `n`. (the entire array is a heap)
2. Swap the ROOT and the last item, then decrease the heap area by one.
    1. The root was the biggest element (inside the heap area).
    2. Now the biggest element is at the end (the correct place in the sorted array).
3. Heapify Down to fix the ROOT.
4. `if (heapArea > 1) { return to step 2 } else { HALT }`
    1. In the handwritten note I put `heapArea > 0`, but if only one element remains, this is the smallest one.
*/


// O(n*log n)
function heapsort(items) {
  buildHeapInPlace(items); // 1. build heap in place using same array

  let heapAreaStart = 0;
  let heapAreaEnd = items.length - 1;

  // O(n)
  while (heapAreaEnd > 1) { // step 4 if (heapArea > 1) { return to step 2 } else { HALT }
    swap(items, 0, heapAreaEnd); // step 2 - swap root and last heap item
    --heapAreaEnd; // step 2.1 - decrease heap area
    // O(log n)
    heapifyDown({ items, currItemPosition: 0, lastHeapIndex: heapAreaEnd }); // step 3 heapify down to fix the new root (put in the right place)
  }
  // return items; // not needed since heapsort is in place
}

// different from build heap in place, in heapsort we start with a maxheap and then we swap and fix top-bottom
// not bottom-top as in build heap in place
// it makes n * log n operations instead of converging to O(n) as in build heap in place

const arr = [1, 2, 9, 4, 5, 6];
console.log('before: arr', arr)
heapsort(arr)
console.log('after arr', arr)