import { TreeNode } from './../dsa-in-js/binary-tree.js'

// time O(nodes), space O(height tree) -> because of stack
function recursiveDfs(currNode) {
  if (!currNode) return;
  console.log(currNode.val);
  recursiveDfs(currNode.left);
  recursiveDfs(currNode.right);
}

// iterative is safer --> no call stack limit AND
// slightly faster -> no recursion state overhead
// time O(nodes), space O(height tree) -> same as recursive
const stack = []
function iterativeDfs(currNode) {
  if (!currNode) return;
  stack.push(currNode);

  while (stack.length > 0) {
    const item = stack.pop();
    console.log(item.val);

    if (item.right != null) {
      stack.push(item.right);
    }
    if (item.left != null) {
      stack.push(item.left);
    }
  }
}

const root = TreeNode.treeFromArray([3, 9, 20, null, null, 15, 7]);
console.log('root', root)
console.log('recursive:')
recursiveDfs(root) // 3, 9, 20, 15, 7
console.log('\niterative:')
iterativeDfs(root) // 3, 9, 20, 15, 7
