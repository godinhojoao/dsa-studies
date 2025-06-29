typedef struct {
  int count;
  char ch;
} Item;

struct Heap {
  Item data[3];  // char + occurrences of this char (a,b,c)
  int size;
} typedef Heap;

void swap(Item* a, Item* b) {
  Item t = *a;
  *a = *b;
  *b = t;
}

void heapInsertFix(Heap* heap, int i) {  // after insert we break heap's rule, and we need to adjust
  int parent = (i - 1) / 2;
  if(i > 0 && heap->data[i].count > heap->data[parent].count) {  // current value > parent value (invalid since I need a max heap)
    swap(&heap->data[i], &heap->data[parent]);  // swap
    heapInsertFix(heap, parent);  // test again
  }
}

void maxHeapRemoveFix(Heap* heap, int i) {
  int leftChildIndex = 2 * i + 1;
  int rightChildIndex = 2 * i + 2;
  int max = i;  // index with max value between the i parent and its childen "left" and "right"
  // im trying to make a "maxHeap" where we have biggest as parent

  if(leftChildIndex < heap->size && heap->data[leftChildIndex].count > heap->data[max].count) max = leftChildIndex;
  if(rightChildIndex < heap->size && heap->data[rightChildIndex].count > heap->data[max].count) max = rightChildIndex;

  if(max != i) {  // if the max isn't the i, heap is wrong, fix it
    swap(&heap->data[i], &heap->data[max]);  // swap to fix
    maxHeapRemoveFix(heap, max);  // test again until it be fixed
  }
}

void insert(Heap* heap, char ch, int count) {
  if(heap->size >= 3 || count <= 0) return;
  heap->data[heap->size].ch = ch;
  heap->data[heap->size].count = count;
  heapInsertFix(heap, heap->size);
  heap->size++;
}

Item getFirst(Heap* heap) {  // "pop" of stack or "dequeue", but on heap
  Item val = heap->data[0];  // temp of our root to return the biggest item (biggest value)
  heap->data[0] = heap->data[heap->size - 1];  // our root now is the last added
  heap->size--;  // our size is decreased
  maxHeapRemoveFix(heap, 0);  // now our heap is wrong we need to fix it again (our root is wrong)
  return val;  // return biggest added value (old root)
}

char* longestDiverseString(int a, int b, int c) {
  // starting heap with max occurrences of each (heap will sort it correctly because it's a maxHeap)
  Heap heap;
  heap.size = 0;
  insert(&heap, 'a', a);
  insert(&heap, 'b', b);
  insert(&heap, 'c', c);

  int recurrent = 0;  // used to avoid >2 occurrences
  char lastChar;  // used to >2 occurrences
  char* myString = (char*)malloc(sizeof(char) * (a + b + c + 1));  // string to return + \0
  int currentPhraseSize = 0;


  // Time complexity: O(a+b+c) since heap size is a+b+c in worst case
  while(heap.size > 0) {  // every time we decrease 1 in heap size after concatenating a char, so if we still have >0 size we stay on while
    Item biggest = getFirst(&heap);  // maxHeap first == root, that is the biggest (char with more occurrences on heap)

    if(recurrent == 2 && biggest.ch == lastChar) {
      if(heap.size == 0) break;  // if we've used this char twice already and this was the last, we need to break

      Item secondBiggest = getFirst(&heap);
      myString[currentPhraseSize++] = secondBiggest.ch;
      lastChar = secondBiggest.ch;

      // occurrences of same last char are 1 or 2?
      recurrent = (currentPhraseSize >= 2 && lastChar == myString[currentPhraseSize - 2]) ? 2 : 1;
      secondBiggest.count--;  // remove since we've added once
      // if we still have more of this char, we insert on heap again, since getFirst remove the entire charItem from heap
      if(secondBiggest.count > 0) insert(&heap, secondBiggest.ch, secondBiggest.count);
      insert(&heap, biggest.ch, biggest.count);
    } else {
      myString[currentPhraseSize++] = biggest.ch;
      lastChar = biggest.ch;

      // occurrences of same last char are 1 or 2?
      recurrent = (currentPhraseSize >= 2 && lastChar == myString[currentPhraseSize - 2]) ? 2 : 1;
      biggest.count--;  // remove since we've added once
      // if we still have more of this char, we insert on heap again, since getFirst remove the entire charItem from heap
      if(biggest.count > 0) insert(&heap, biggest.ch, biggest.count);
    }
  }

  myString[currentPhraseSize] = '\0';
  return myString;
}

#include <stdio.h>
#include <stdlib.h>

int main() {
  int a = 7, b = 1, c = 0;

  char* res = longestDiverseString(a, b, c);
  printf("res: %s\n", res);

  free(res);
  return 0;
}