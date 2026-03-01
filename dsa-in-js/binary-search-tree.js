import { BinaryTreeNode } from './binary-tree.js'

export class BinarySearchTreeNode extends BinaryTreeNode {

  // space complexity: O(height)
  // time complexity: O(height)
  // if bst is balanced: height = log n; if not balanced height = n;
  static insert(currNode, val) {
    if (val <= currNode.val) {
      if (currNode.left == null) {
        currNode.left = new BinarySearchTreeNode(val);
        return;
      }
      return BinarySearchTreeNode.insert(currNode.left, val);
    }

    if (val > currNode.val) {
      if (currNode.right == null) {
        currNode.right = new BinarySearchTreeNode(val);
        return;
      }
      return BinarySearchTreeNode.insert(currNode.right, val);
    }
  }

  // time O(height)
  // space O(height)
  recursiveSearch(val) {
    if (val == this.val) {
      return this;
    }

    if (val < this.val && !!this.left) {
      return this.left.recursiveSearch(val);
    }

    if (val > this.val && !!this.right) {
      return this.right.recursiveSearch(val);
    }

    return null; // not find
  }

  // time O(height)
  // space O(1)
  static iterativeSearch(currNode, val) {
    while (currNode) {
      if (currNode.val === val) return currNode;

      if (val < currNode.val) {
        currNode = currNode.left;
      } else {
        currNode = currNode.right;
      }
    }
    return null;
  }
}

/*

    5
  3   10
2        15
       11
*/

const root = new BinarySearchTreeNode(5);
BinarySearchTreeNode.insert(root, 10);
BinarySearchTreeNode.insert(root, 15);
BinarySearchTreeNode.insert(root, 3);
BinarySearchTreeNode.insert(root, 2);
BinarySearchTreeNode.insert(root, 11);

console.log('root', root)

console.log('root.recursiveSearch(11)', root.recursiveSearch(11))
console.log('root.recursiveSearch(999)', root.recursiveSearch(999))

console.log('BinarySearchTreeNode.iterativeSearch(root, 11)', BinarySearchTreeNode.iterativeSearch(root, 11))
console.log('BinarySearchTreeNode.iterativeSearch(root, 999)', BinarySearchTreeNode.iterativeSearch(root, 999))



