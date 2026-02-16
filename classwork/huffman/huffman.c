/*
Deleting the executable after running:
gcc huffman.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOLS_AMOUNT 256

#define INPUT_FILE_PATH "./palavras.txt"
#define COMPRESSED_OUTPUT_PATH "./compressed_output.txt"
#define DECOMPRESSED_OUTPUT_PATH "./decompressed_output.txt"

struct Node {
  struct Node *left, *right;
  unsigned char currChar;  // 0-255 instead of 127 negatives + 0 + 127 positives
  int occurrences;
} typedef Node;

struct MinHeap {
  Node* items[SYMBOLS_AMOUNT];
  int currLength;
  int maxLength;
} typedef MinHeap;

void swap(MinHeap* heap, int indexA, int indexB) {
  Node* tempA = heap->items[indexA];
  heap->items[indexA] = heap->items[indexB];
  heap->items[indexB] = tempA;
}

void heapifyUp(MinHeap* heap, int itemIndex) {
  Node* addedItem = heap->items[itemIndex];

  int parentIndex = ((itemIndex - 1) / 2) >= 0 ? ((itemIndex - 1) / 2) : -1;
  if(parentIndex == -1 || itemIndex == parentIndex) {
    return;
  }

  Node* addedItemParent = heap->items[parentIndex];

  int isMinHeapInvalid = addedItemParent->occurrences > addedItem->occurrences;
  if(isMinHeapInvalid) {
    swap(heap, itemIndex, parentIndex);
    heapifyUp(heap, parentIndex);  // now we need to check if parent is valid or not, and if not fix
  }
}

void minHeapInsert(MinHeap* heap, Node* item) {
  if(heap->currLength + 1 > heap->maxLength) {
    printf("internal error heap is full increase symbols amount or debug it hehe...\n");
    exit(1);
  }
  heap->items[heap->currLength] = item;  // add to the end

  if(heap->currLength > 1) {
    heapifyUp(heap, heap->currLength);
  }
  heap->currLength++;
}

void heapifyDown(MinHeap* heap, int i) {
  int leftChildIndex = 2 * i + 1;
  int rightChildIndex = 2 * i + 2;
  int smallestIndex = i;

  if(leftChildIndex < heap->currLength &&
     heap->items[leftChildIndex]->occurrences < heap->items[smallestIndex]->occurrences) {
    smallestIndex = leftChildIndex;
  }

  if(rightChildIndex < heap->currLength &&
     heap->items[rightChildIndex]->occurrences < heap->items[smallestIndex]->occurrences) {
    smallestIndex = rightChildIndex;
  }

  if(smallestIndex != i) {
    swap(heap, i, smallestIndex);
    heapifyDown(heap, smallestIndex);
  }
}

Node* minHeapPop(MinHeap* heap) {
  if(heap->currLength == 0) {
    printf("can't pop, empty heap;\n");
    return NULL;
  }

  Node* minItemPtr = heap->items[0];
  heap->currLength--;

  if(heap->currLength > 0) {
    heap->items[0] = heap->items[heap->currLength];  // remove from tree and put last in the place
    heapifyDown(heap, 0);  // fix from top down since we've added the last item as the root in the top
  }

  return minItemPtr;
}

void getCharOcurrences(int* charOcurrences) {
  FILE* file = fopen(INPUT_FILE_PATH, "r");
  if(!file) {
    printf("getCharOcurrences error reading file\n");
    exit(1);
  }

  unsigned char currChar;
  while(fread(&currChar, sizeof(unsigned char), 1, file) == 1) {
    charOcurrences[currChar]++;
  }
  fclose(file);
}

Node* createNode(unsigned char currChar, int occurrences) {
  Node* charRoot = malloc(sizeof(Node));
  charRoot->left = NULL;
  charRoot->right = NULL;
  charRoot->occurrences = occurrences;
  charRoot->currChar = currChar;
  return charRoot;
}

Node* mergeTrees(Node* lessOccurrencesTree, Node* secondTree) {
  if(lessOccurrencesTree == NULL || secondTree == NULL) {
    printf("error merging trees\n");
    exit(1);
  }
  Node* mergedResult = createNode(0, lessOccurrencesTree->occurrences + secondTree->occurrences);
  mergedResult->left = lessOccurrencesTree;
  mergedResult->right = secondTree;
  return mergedResult;
}

Node* buildHuffmanTree(int* charOccurrences) {
  MinHeap heap = {0};
  heap.maxLength = SYMBOLS_AMOUNT;

  // 1st empty nodes for each char and add into the minHeap
  for(int i = 0; i < SYMBOLS_AMOUNT; i++) {
    if(charOccurrences[i] > 0) {
      minHeapInsert(&heap, createNode((unsigned char)i, charOccurrences[i]));
    }
  }

  // 2nd get first two -> merge them -> put on heap again (repeat until heap contains only 1 node)
  while(heap.currLength >= 2) {
    Node* lessOccurrencesTree = minHeapPop(&heap);
    Node* secondTree = minHeapPop(&heap);
    Node* mergedResult = mergeTrees(lessOccurrencesTree, secondTree);

    minHeapInsert(&heap, mergedResult);
  }

  if(heap.currLength == 1) {
    return heap.items[0];  // final root node
  }

  return NULL;
}

void buildCodesTable(Node* currNode, char codesTable[SYMBOLS_AMOUNT][60], char currCode[60], int currCodeStringIndex) {
  // iterate over entire tree -> save code sequence -> find a leaf? save code string on table (dont forget null
  // terminator)
  if(currNode == NULL) {
    printf("reach the end of tree\n");
    return;
  }

  // convetion left = 0, right = 1
  if(currNode->left) {
    currCode[currCodeStringIndex] = '0';
    buildCodesTable(currNode->left, codesTable, currCode, currCodeStringIndex + 1);
  }

  if(currNode->right) {
    currCode[currCodeStringIndex] = '1';
    buildCodesTable(currNode->right, codesTable, currCode, currCodeStringIndex + 1);
  }

  if(currNode->right == NULL && currNode->left == NULL) {  // isleaf
    currCode[currCodeStringIndex] = '\0';
    strcpy(codesTable[(unsigned char)currNode->currChar], currCode);
    // printf("currNode->currChar: %c, currcode: %s\n", currNode->currChar, currCode);
  }
}

void writeSequenceIntoFile(FILE* writeFilePtr, char* currSequence, int* currWrittenBitsAmount) {}

void freeHuffmanTree(Node* currNode) {
  if(currNode == NULL) {
    return;
  }

  // iterate over all tree and free nodes.
  if(currNode->left) {
    freeHuffmanTree(currNode->left);
  }

  if(currNode->right) {
    freeHuffmanTree(currNode->right);
  }

  free(currNode);
}

long int compressFile(char codesTable[SYMBOLS_AMOUNT][60], int charOccurrences[SYMBOLS_AMOUNT]) {
  long int currWrittenBitsAmount = 0;

  FILE* fileReadPtr = fopen(INPUT_FILE_PATH, "r");
  if(!fileReadPtr) {
    printf("compressFile error reading file\n");
    exit(1);
  }
  FILE* fileWritePtr = fopen(COMPRESSED_OUTPUT_PATH, "wb");
  if(!fileWritePtr) {
    printf("compressFile error opening file to write\n");
    exit(1);
  }

  // save charOccurrences on header so we can decompress it
  fwrite(charOccurrences, sizeof(int), SYMBOLS_AMOUNT, fileWritePtr);

  unsigned char buffer = 0;
  int bitsInBuffer = 0;  // how many bits currently in buffer (we need 8bits = 1byte)

  unsigned char currChar;
  while(fread(&currChar, sizeof(unsigned char), 1, fileReadPtr) == 1) {
    char* currSequence = codesTable[currChar];
    int sequenceLength = strlen(currSequence);

    // we need to deal with this as binary so we use one char (currChar)
    // and we handle it using binary operators
    for(int i = 0; i < sequenceLength; i++) {
      buffer <<= 1;  // shift left to open space for next bit
      if(currSequence[i] == '1') {
        buffer |= 1;  // set less significant bit as 1
      }
      bitsInBuffer++;
      currWrittenBitsAmount++;

      if(bitsInBuffer == 8) {  // buffer full -> 1 bytes (valid) -> write on file and clean buffer
        fwrite(&buffer, sizeof(char), 1, fileWritePtr);
        buffer = 0;
        bitsInBuffer = 0;
      }
    }
  }

  // if it was not with one byte before
  // fill the missing bits until reach 8 bits and then write
  if(bitsInBuffer > 0) {
    buffer <<= (8 - bitsInBuffer);  // shift remaining bits to left
    fwrite(&buffer, sizeof(char), 1, fileWritePtr);
  }

  fclose(fileReadPtr);
  fclose(fileWritePtr);

  return currWrittenBitsAmount;
}

void decompressFile(long int originalBitsAmount) {
  FILE* fileReadPtr = fopen(COMPRESSED_OUTPUT_PATH, "rb");
  if(!fileReadPtr) {
    printf("decompressFile error reading file\n");
    exit(1);
  }
  FILE* fileWritePtr = fopen(DECOMPRESSED_OUTPUT_PATH, "wb");
  if(!fileWritePtr) {
    printf("decompressFile error opening file to write\n");
    exit(1);
  }

  // 1st read header, get occurrences
  int charOccurrences[SYMBOLS_AMOUNT] = {};
  fread(charOccurrences, sizeof(int), SYMBOLS_AMOUNT, fileReadPtr);

  // 2nd build huffman's tree
  Node* huffmanTreeRoot = buildHuffmanTree(charOccurrences);
  if(huffmanTreeRoot == NULL) {
    printf("decompress error building huffman tree");
    exit(1);
  }

  // 3rd for each bit on sequence iterate over the tree if find leaf write char and start new sequence
  unsigned char buffer;
  Node* currNodeForSequence = huffmanTreeRoot;

  long int bitsRead = 0;

  while(bitsRead < originalBitsAmount && fread(&buffer, sizeof(unsigned char), 1, fileReadPtr) == 1) {
    // read from MSB to LSB -> same order used to compress
    for(int i = 7; i >= 0 && bitsRead < originalBitsAmount; i--) {
      int bit = (buffer >> i) & 1;
      // verify value of bit 1, making & 1 (if it was 0 return 0, if was 1 return 1)
      // logic identity law: a && true = a

      if(bit == 0) {
        currNodeForSequence = currNodeForSequence->left;
      } else {
        currNodeForSequence = currNodeForSequence->right;
      }

      // if leaf -> write char and start reading again from root
      int isLeaf = currNodeForSequence->left == NULL && currNodeForSequence->right == NULL;

      if(isLeaf) {
        fwrite(&currNodeForSequence->currChar, sizeof(char), 1, fileWritePtr);
        currNodeForSequence = huffmanTreeRoot;
      }

      bitsRead++;
    }
  }

  freeHuffmanTree(huffmanTreeRoot);
  fclose(fileReadPtr);
  fclose(fileWritePtr);
}

long int getFileSize(char* path) {
  FILE* filePtr = fopen(path, "rb");
  if(!filePtr) {
    printf("getFileSize error reading file\n");
    exit(1);
  }

  fseek(filePtr, 0, SEEK_END);     // go to end
  long int size = ftell(filePtr);  // get size in bytes

  fclose(filePtr);
  return size;
}

int main() {
  // 1st get char occurrences from file
  int charOccurrences[SYMBOLS_AMOUNT] = {};
  getCharOcurrences(charOccurrences);

  // 2nd build huffman's tree
  Node* huffmanTreeRoot = buildHuffmanTree(charOccurrences);
  if(huffmanTreeRoot == NULL) {
    printf("error building huffman tree");
    exit(1);
  }

  // 3rd mount the table for every symbol and its corresponding sequences
  char codesTable[SYMBOLS_AMOUNT][60] = {0};
  char currCode[60] = {0};
  buildCodesTable(huffmanTreeRoot, codesTable, currCode, 0);

  // 4th compress file (use table for it -> faster than the huffman's tree -> char to code in O(1))
  long int writtenBitsAmount = compressFile(codesTable, charOccurrences);

  // 5th decompress file
  decompressFile(writtenBitsAmount);

  long int inputFileSize = getFileSize(INPUT_FILE_PATH);
  long int decompressedFileSize = getFileSize(DECOMPRESSED_OUTPUT_PATH);
  long int compressedFileSize = getFileSize(COMPRESSED_OUTPUT_PATH);

  printf("Input file size: %ld bytes\n", inputFileSize / 8);
  printf("Decompressed file size: %ld bytes\n", decompressedFileSize / 8);
  printf("Compressed file size: %ld bytes\n", compressedFileSize / 8);

  freeHuffmanTree(huffmanTreeRoot);

  printf(inputFileSize == decompressedFileSize ? "input file size == compressed file size"
                                               : "something wrong.. inputFileSizeInBytes != decompressedFileSize\n");
}
