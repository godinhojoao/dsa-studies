/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
void insertSortedVal(int** sortedArray, int val) { // not done yet
    int newArraylength = (sizeof(*sortedArray) / sizeof(int)) +1;
    *sortedArray = realloc(*sortedArray, sizeof(int) * newArrayLength);
    
    if (newArrayLength == 1) {
        *sortedArray[0] = val;
        return;
    }

    for (int i = 0; i < newArrayLength; i++) {
        if (*sortedArray[i] > val) {
            // insert before this index and break
            int temp = *sortedArray[i];
            *sortedArray[i] = val;
            //all items after this one should go +1 position to right
            break;
        }
    }
}

void traverseTreeAndGetSortedValues(struct TreeNode* treeNode, int** sortedArray) {
    if((treeNode == NULL) || (treeNode && treeNode.val == NULL)) {
        return;
    } else {
        insertSortedVal(sortedArray, treeNode->val);
    }
}

struct TreeNode* balanceBST(struct TreeNode* root) {
    /*
    1 - traverse all tree and create a sorted array: time and space complexity both: O(n)
    2 - use this array to create another BST (balanced): time and space complexity both: O(n)
    final BigO space and time complexity O(n)
    
    com certeza nao é a solução mais eficiente, a mais eficiente deve fazer só swap na arvore existente
    */
    int* sortedArray = malloc(sizeof(int));
    traverseTreeAndGetSortedValues(root, &sortedArray); // not done yet

    // here I'll have the sorted array and just have to recreate the BST but balanced
    
   

    return root;
}

