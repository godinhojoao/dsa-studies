#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
  int val;
  struct TreeNode* left;
  struct TreeNode* right;
};

struct TreeNode* initNode(int val) {
  struct TreeNode* node = malloc(sizeof(struct TreeNode));
  node->left = NULL;
  node->right = NULL;
  node->val = val;
  return node;
}

void binarySearchTreeInsert(struct TreeNode* node, int val) {
  if(node == NULL) {
    return;
  }
  if(!node->val && node->val != 0) {
    node->val = val;
    return;
  }

  if(val > node->val) {
    if(node->right == NULL) {
      node->right = initNode(val);
    } else {
      binarySearchTreeInsert(node->right, val);
    }
  } else {
    if(node->left == NULL) {
      node->left = initNode(val);
    } else {
      binarySearchTreeInsert(node->left, val);
    }
  }
}

void inOrderTraverseSavingSortedValues(struct TreeNode* treeNode, int** sortedArray, int* size) {
  if(treeNode == NULL) {
    return;
  }
  inOrderTraverseSavingSortedValues(treeNode->left, sortedArray, size);

  *size += 1;
  *sortedArray = realloc(*sortedArray, sizeof(int) * (*size));
  (*sortedArray)[*size - 1] = treeNode->val;

  inOrderTraverseSavingSortedValues(treeNode->right, sortedArray, size);
}

void insertDividingOnMiddleEveryTime(int start, int end, int* sortedArray, struct TreeNode* root) {
  if(start > end) {
    return;
  }
  int mid = (start + end) / 2;
  binarySearchTreeInsert(root, sortedArray[mid]);
  insertDividingOnMiddleEveryTime(start, mid - 1, sortedArray, root);
  insertDividingOnMiddleEveryTime(mid + 1, end, sortedArray, root);
}

// O(n) time and space
struct TreeNode* balanceBST(struct TreeNode* root) {
  int* sortedArray = NULL;
  int arraySize = 0;

  inOrderTraverseSavingSortedValues(root, &sortedArray, &arraySize);

  if(arraySize <= 2) {
    return root;
  }

  int midIndex = arraySize / 2;
  struct TreeNode* newRoot = initNode(sortedArray[midIndex]);

  insertDividingOnMiddleEveryTime(0, midIndex - 1, sortedArray, newRoot);
  insertDividingOnMiddleEveryTime(midIndex + 1, arraySize - 1, sortedArray, newRoot);

  freeTree(root);
  free(sortedArray);

  return newRoot;
}

void printInOrder(struct TreeNode* root) {
  if(!root) return;
  printInOrder(root->left);
  printf("%d ", root->val);
  printInOrder(root->right);
}

void freeTree(struct TreeNode* root) {
  if(!root) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

int main() {
  int values[] = {1, 15, 14, 17, 7, 2, 12, 3, 9, 11};
  int n = sizeof(values) / sizeof(values[0]);

  struct TreeNode* root = initNode(values[0]);
  for(int i = 1; i < n; i++) {
    binarySearchTreeInsert(root, values[i]);
  }

  printf("In-order before balancing:\n");
  printInOrder(root);
  printf("\n");

  struct TreeNode* balanced = balanceBST(root);

  printf("In-order after balancing:\n");
  printInOrder(balanced);
  printf("\n");

  freeTree(balanced);
  return 0;
}
