import { TreeNode } from './binary-tree.js'

// time O(nodes), space O(width of tree)
const queue = [] // FIFO
function bfs(currNode) {
  if (!currNode) return;
  queue.push(currNode);

  let i = 0;
  while (i < queue.length) {
    const item = queue[i++];
    // get next item in queue without removing it (O(1))
    // (using index instead of shift, which is O(n))
    console.log(item.val);

    if (item.left != null) {
      queue.push(item.left);
    }
    if (item.right != null) {
      queue.push(item.right);
    }
  }
}

const root = TreeNode.treeFromArray([1, 20, 32, 2, 30, 40]);
console.log('root', root)
bfs(root) // 1,20,32,2,30,40
