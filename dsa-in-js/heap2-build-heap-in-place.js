/*
all items after n/2 are leaves: A[n/2+1, ..., n]
as indexes start at 0, n/2 is already a leaf

1. Start from nodes at height 1 up to height log n
  - (leaves are already heaps, so skip height 0; start on height 1)
2. For each node, call heapifyDown
  - Since children were already heapified before their parent, heapifyDown can correctly fix the subtree

# Why do we start on height 1?
-- Because leaves (h=0) are already valid heaps, and heapifyDown only works for a parent with valid heaps as children.
-- this is an assumption of a heap, we never do multiple fixes, we fix only one node at a time.
*/

function swap(items, addedIndex, parentIndex) {
  const temp = items[addedIndex];
  items[addedIndex] = items[parentIndex];
  items[parentIndex] = temp;
}

function heapifyDown({ currItemPosition, items }) {
  let mostPriorityItemIndex = currItemPosition;
  const leftChildIndex = 2 * currItemPosition + 1;
  const rightChildIndex = 2 * currItemPosition + 2;

  const leftChildExists = leftChildIndex < items.length;
  if (leftChildExists && items[leftChildIndex] > items[mostPriorityItemIndex]) {
    mostPriorityItemIndex = leftChildIndex;
  }

  const rightChildExists = rightChildIndex < items.length;
  if (rightChildExists && items[rightChildIndex] > items[mostPriorityItemIndex]) {
    mostPriorityItemIndex = rightChildIndex;
  }

  // if parent already has the highest priority, do nothing
  if (mostPriorityItemIndex != currItemPosition) {
    swap(items, currItemPosition, mostPriorityItemIndex);
    heapifyDown({ items, currItemPosition: mostPriorityItemIndex });
  }
}

function pop(items) {
  const currLength = items.length;
  if (currLength == 0) { return null; }

  const itemToReturn = items[0];
  items[0] = items[currLength - 1]; // last item is new root
  items.pop(); // remove last item from array

  if (currLength > 1) {
    heapifyDown({ items, currItemPosition: 0 }); // fix top down
  }

  return itemToReturn;
}

// O(n)
function buildHeapInPlace(arr) {
  const arrSize = arr.length;
  for (let i = Math.floor(arrSize / 2) - 1; i >= 0; i--) {
    heapifyDown({ items: arr, currItemPosition: i });
  }
}

const arr = [1, 2, 9, 4, 5, 6];
buildHeapInPlace(arr);
console.log('heap', arr)

let i = 0;
while (arr.length) {
  console.log(`${i+1} - pop(arr): ${pop(arr)}`)
  ++i;
}

// why is it O(n)? Watch starting at 30:00 - [MIT OpenCourseWare - Lecture 4: Heaps and Heap Sort (Srini Devadas, 2011)](https://www.youtube.com/watch?v=B7hVxCmfPtM)