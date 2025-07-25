// put code here and run docker
// docker build -t valgrind-test-runner . && docker run valgrind-test-runner && docker run --rm valgrind-test-runner

/*
Deleting the executable after running:
gcc binarySearchTree.c -o test && ./test && rm -rf ./test

// In Big O comments, n = nodes of tree
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
  int val;
  struct TreeNode* left;
  struct TreeNode* right;
};

// O(1)
struct TreeNode* initNode() {
  struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
  node->val = 0;
  node->left = NULL;
  node->right = NULL;
  return node;
}

// If balanced BST: O(log n)
// If unbalanced BST: O(n)
int insert(struct TreeNode* node, int val) {
  if(!node->val) {
    node->val = val;
    return 1;
  } else if(val >= node->val) {
    if(node->right == NULL) {
      node->right = initNode();
      node->right->val = val;
    } else {
      insert(node->right, val);
    }
  } else if(val < node->val) {
    if(node->left == NULL) {
      node->left = initNode();
      node->left->val = val;
    } else {
      insert(node->left, val);
    }
  }

  return 1;
}

// If balanced BST: O(log n)
// If unbalanced BST: O(n)
int findMin(struct TreeNode* node) {
  if(node == NULL) {
    return -1;
  }

  if(node->left == NULL) {
    return node->val;
  } else {
    return findMin(node->left);
  }
}

// If balanced BST: O(log n)
// If unbalanced BST: O(n)
int findMax(struct TreeNode* node) {
  if(node == NULL) {
    return -1;
  }

  if(node->right == NULL) {
    return node->val;
  } else {
    return findMax(node->right);
  }
}

// If balanced BST: O(log n)
// If unbalanced BST: O(n)
struct TreeNode* search(struct TreeNode* node, int valToFind) {
  if(node == NULL) {
    printf("Value %d not found on BST: NULL\n", valToFind);
    return NULL;
  }
  if(node->val == valToFind) {
    printf("Value %d found on BST!\n", node->val);
    return node;
  }

  if(valToFind < node->val) {
    return search(node->left, valToFind);
  } else {
    return search(node->right, valToFind);
  }
}

// O(n)
void inOrderTraverse(struct TreeNode* node) {
  if(node == NULL) {
    return;
  }
  inOrderTraverse(node->left);
  if(node->val > 0) {  // Only printing values > 0; not a BST requirement
    printf("Value: %d \n", node->val);
  }
  inOrderTraverse(node->right);
}

void clearTree(struct TreeNode* node) {
  if(node == NULL) {
    return;
  }
  clearTree(node->left);
  clearTree(node->right);
  free(node);
}

int main() {
  struct TreeNode* root = initNode();
  insert(root, 20);  // root
  insert(root, 9);  // left
  insert(root, 32);  // right
  inOrderTraverse(root);  // 9, 20, 32

  int min = findMin(root);
  printf("min: %d\n", min);

  int max = findMax(root);
  printf("max: %d\n", max);

  search(root, 10);  // not found
  search(root, 9);  // found

  clearTree(root);

  return 0;
}
