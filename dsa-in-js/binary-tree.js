export class TreeNode {
  constructor(val, left, right) {
    this.val = val ?? 0; // if null or undefined use 0
    this.left = left ?? null;
    this.right = right ?? null;
  }

  static treeFromArray(items) {
    if (!items.length || items[0] == null) return null;
    const root = new TreeNode(items[0])
    const nodes = [root];

    for (let i = 0; i < items.length; i++) {
      const leftChildIdx = (2 * i) + 1;
      const rightChildIdx = (2 * i) + 2;

      if (items[leftChildIdx] != null) {
        nodes[i].left = new TreeNode(items[leftChildIdx]);
        nodes[leftChildIdx] = nodes[i].left;
      }

      if (items[rightChildIdx] != null) {
        nodes[i].right = new TreeNode(items[rightChildIdx]);
        nodes[rightChildIdx] = nodes[i].right;
      }
    }

    return root;
  }
}
// const root = TreeNode.treeFromArray([3, 9, 20, null, null, 15, 7]);
// console.log('root', root)

