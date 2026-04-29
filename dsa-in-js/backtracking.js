const n = 9;

const arr = [];
for (let i = 1; i <= n; i++) {
  arr.push(i);
}
console.log('arr', arr)

// Backtracking is generally used for brute-force (and works as DFS)
// - choose -> explore -> undo (if isn't correct path) -> try next choice
// - It doesn't mean you always have to test all possible solutions
// - Sometimes you can identify that the path isn't the right one before reaching end, stop in the middle and go to test another path. (prune case)
// - used to generate all possible solutions (n * (n-1) * (n-2) * ... * 1) = n! (all permutations of n elements)
// # Why Backtracking follows DFS idea? because it test one entire path (depth) before trying another.

// It isn't the fastest backtracking, but it is the easier to understand:
// O(n!) time complexity
// O(n) auxiliary space -> O(n) visited array + O(n) recursion stack = O(2n) = O(n)
// we could make it using O(1) auxiliary space, with swap in place. But still same complexity

console.time(`listPermutations for n = ${n}`)
function listPermutations(items, visitedItems) {
  if (items.length == visitedItems.length) {
    return console.log('permutation', visitedItems); // "base case" of recursion -> permutation with all items (end path)
  }

  for (let i = 0; i < items.length; i++) {
    if (!visitedItems.includes(items[i])) {
      visitedItems.push(items[i]);
      listPermutations(items, visitedItems);
      visitedItems.pop(); // one path of recursion finished above, undo (return), backtrack to check other cases
    }
  }
}

listPermutations(arr, []);
console.timeEnd(`listPermutations for n = ${n}`)

/*
Backtracking permutations:
1. Base case: (if all items are used -> print permutation)
2. Try all choices: loop through all items
3. Choose: if not used -> add to current path
4. Recurse: go deeper with current choice
5. Undo (backtrack): remove last item to try next option
- NOTE: you could use a while and "remaining array" instead of a for loop and "visited items"

This builds a DFS tree of all permutations.
For more info check the image within this same folder; file: backtracking.png
*/