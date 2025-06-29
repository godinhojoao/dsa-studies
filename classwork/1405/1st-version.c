struct Heap {
  char* data; // strings
  int size;
} typedef Heap;

void swap(char* a, char* b) {
  char t = *a;
  *a = *b;
  *b = t;
}

void heapInsertFix(Heap* heap, int i) {  // after insert we break heap's rule, and we need to adjust
  int parent = (i - 1) / 2;
  if(i > 0 && heap->data[i] > heap->data[parent]) {  // current value > parent value (invalid since I need a max heap)
    swap(&heap->data[i], &heap->data[parent]);  // swap
    heapInsertFix(heap, parent);  // test again
  }
}

void maxHeapFix(Heap* heap, int i) {
  int leftChildIndex = 2 * i + 1;
  int rightChildIndex = 2 * i + 2;
  int max = i; // index with max value between the i parent and its childen "left" and "right"
  // im trying to make a "maxHeap" where we have biggest as parent
  
  if(leftChildIndex < heap->size && heap->data[leftChildIndex] > heap->data[max]) max = leftChildIndex;
  if(rightChildIndex < heap->size && heap->data[rightChildIndex] > heap->data[max]) max = rightChildIndex;
  
  if(max != i) { // if the max isn't the i, heap is wrong, fix it
    swap(&heap->data[i], &heap->data[max]); // swap to fix
    maxHeapFix(heap, max); // test again until it be fixed
  }
}

void insert(Heap* heap, int val) {
  if(heap->size >= 3) return;
  heap->data[heap->size] = val;
  heapInsertFix(heap, heap->size);
  heap->size++;
}

int getFirst(Heap* heap) { // "pop" of stack or "dequeue", but on heap
  if(heap->size == 0) return -1;
  int val = heap->data[0]; // temp of our root to return the first item (biggest value)
  heap->data[0] = heap->data[heap->size - 1]; // our root now is the last added
  heap->size--; // our size is decreased
  maxHeapFix(heap, 0); // now our heap is wrong we need to fix it again (our root is wrong)
  return val; // return first added value (old root)
}

char* longestDiverseString(int a, int b, int c) {
  char aChar = 'a';
  char bChar = 'b';
  char cChar = 'c';
  int recurrent = 0;

  char charWithBiggestAmount = a>b && a>c ? 'a' : b>a && b>c ? 'b' : 'c';
  char secondBiggest = (a > b && a < c) || (a > c && a<b) ? 'a' : 'b';
  if ((b > a && b < c) || (b > c && b < a)) {
    secondBiggest = 'b';
  } else if ((c > a && c < b) || (c > b && c < a)) {
    secondBiggest = 'c';
  }

  char smallestChar = 'a';
  if (charWithBiggestAmount == 'a' && secondBiggest == 'b') {
    smallestChar = 'c';
  } else if(charWithBiggestAmount == 'a' && secondBiggest == 'c') {
    smallestChar = 'b';
  } else if (charWithBiggestAmount == 'b' && secondBiggest == 'a') {
    smallestChar = 'c';
  } else if (charWithBiggestAmount == 'b' && secondBiggest == 'c') {
    smallestChar = 'a';
  } else if (charWithBiggestAmount == 'c' && secondBiggest == 'a') {
    smallestChar = 'b';
  } else if (charWithBiggestAmount == 'c' && secondBiggest == 'b') {
    smallestChar = 'a';
  }

  char currentChar = charWithBiggestAmount; // starts with the biggest, then goes to the 2nd...
  int currentPhraseSize = 0;
  char* myString = (char*)malloc(sizeof(char) * (a + b + c));

  // i need to make it work with the heap instead of using only the while with one string
  while (a >0 || b>0 || c>0) {
    recurrent++;
    myString[currentPhraseSize] = currentChar;
    currentPhraseSize += 1;

    if (currentChar == 'a') {
      a -= 1;
    } else if(currentChar == 'b') {
      b -= 1;
    } else {
      c -= 1;
    }

    if (recurrent == 2) {
      recurrent = 0;

      int biggestMissing = charWithBiggestAmount == 'a' ? a : charWithBiggestAmount == 'b' ? b : c;
      int secondMissing = secondBiggest == 'a' ? a : secondBiggest == 'b' ? b : c;
      int smallestMissing = smallestChar == 'a' ? a : smallestChar == 'b' ? b : c;
      if (currentChar == charWithBiggestAmount && secondMissing > 0) {
        currentChar = secondBiggest;
      } else {}

      currentChar = ;// second biggest
    }
  }
}