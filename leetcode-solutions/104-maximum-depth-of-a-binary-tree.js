// https://leetcode.com/problems/maximum-depth-of-binary-tree/description/

// recursive solution
// time O(nodes), space O(height tree) -> because of stack
function dfs(currNode) {
  if (!currNode) return 0;
  const left = dfs(currNode.left);
  const right = dfs(currNode.right);

  return 1 + Math.max(left, right);
}

var maxDepth = function (root) {
  if (!root) return 0;
  return dfs(root, 1); // root alone height=1
};


// just for testing
import { TreeNode } from './../dsa-in-js/binary-tree.js'
const root = TreeNode.treeFromArray([3, 9, 20, null, null, 15, 7]);
console.log('maxDepth(root)', maxDepth(root))