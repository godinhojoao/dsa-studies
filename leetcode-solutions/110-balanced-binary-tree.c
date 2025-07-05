// https://leetcode.com/problems/balanced-binary-tree/
// More about it on ./dsa-in-c/AVL-tree.c
int checkBalance(struct TreeNode* node) {
  if(!node) {
    return 0;
  }

  int currentLeftDepth = checkBalance(node->left);
  if(currentLeftDepth == -1) {
    return -1;
  }

  int currentRightDepth = checkBalance(node->right);
  if(currentRightDepth == -1) {
    return -1;
  }

  int depth = currentLeftDepth - currentRightDepth;
  int isNotBalanced = depth > 1 || depth < -1;
  if(isNotBalanced) {
    return -1;
  }

  return (currentLeftDepth > currentRightDepth ? currentLeftDepth : currentRightDepth) + 1;
}

bool isBalanced(struct TreeNode* node) {
  return !(checkBalance(node) == -1);
}