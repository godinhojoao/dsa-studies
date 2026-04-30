// ======================== START - backtracking complexity and logic ========================
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
function listPermutations(items, visitedItems) { // items = arr without repeated values
  if (items.length == visitedItems.length) {
    return console.log('listPermutations example:', visitedItems); // "base case" of recursion -> permutation with all items (end path)
  }

  for (let i = 0; i < items.length; i++) {
    if (!visitedItems.includes(items[i])) {
      visitedItems.push(items[i]);
      listPermutations(items, visitedItems);
      visitedItems.pop(); // one path of recursion finished above, undo (return), backtrack to check other cases
    }
  }
}
listPermutations([1, 2, 3], []);

// Backtracking permutations:
// 1. Base case: (if all items are used -> print permutation)
// 2. Try all choices: loop through all items
// 3. Choose: if not used -> add to current path
// 4. Recurse: go deeper with current choice
// 5. Undo (backtrack): remove last item to try next option
// - NOTE: you could use a while and "remaining array" instead of a for loop and "visited items"

// This builds a DFS tree of all permutations.

// For more info check the image within this same folder; file: backtracking0.png
// ======================== END - backtracking complexity and logic ========================

// Now that we understood backtracking, let's check some cases from combinatorics on code

// 1) k positions, each position independently chooses from n values, values can repeat across positions, formula: n^k
function arrangementWithRepetitionAllowed(items, visitedPath, endPathCount) {
  if (items.length == visitedPath.length) {
    console.log(visitedPath)
    return ++endPathCount;
  }

  for (let i = 0; i < items.length; i++) {
    visitedPath.push(items[i]); // no condition, test all possible positions
    endPathCount = arrangementWithRepetitionAllowed(items, visitedPath, endPathCount);
    visitedPath.pop();
  }
  return endPathCount;
}
// arr = [1,2,1], n = 3, n^k = 3^3 = 27
const arrangementWithRepetitionAllowedArray = [1, 2, 1];
console.log('\narrangementWithRepetitionAllowedArray', arrangementWithRepetitionAllowedArray)
console.log("arrangementWithRepetitionAllowed count: ", arrangementWithRepetitionAllowed(arrangementWithRepetitionAllowedArray, [], 0));

// 2) use all n *distinct* items exactly once, only changing their order, no reuse, formula: n!
function permutationDistinctElements(items, visitedPath, endPathCount) {
  if (items.length == visitedPath.length) {
    console.log(visitedPath);
    return ++endPathCount;
  }

  for (let i = 0; i < items.length; i++) {
    if (visitedPath.includes(items[i])) continue; // ignore already used value on permutation path
    visitedPath.push(items[i]);
    endPathCount = permutationDistinctElements(items, visitedPath, endPathCount);
    visitedPath.pop();
  }
  return endPathCount;
}
// arr = [1,2,3], n = 3, n! = 3! = 6 different paths
const permutationDistinctElementsArray = [1, 2, 3];
console.log('\npermutationDistinctElementsArray', permutationDistinctElementsArray)
console.log("permutationDistinctElements count: ", permutationDistinctElements(permutationDistinctElementsArray, [], 0));

// 3) use all items but some are identical, count unique orders ignoring duplicates, formula: n!/(n1!*n2!*...)
function permuteUnique(nums) {
  const freq = new Map();
  for (const n of nums) {
    const mapKey = n;
    const mapValueCurrentFrequency = freq.get(n) || 0;
    freq.set(mapKey, mapValueCurrentFrequency + 1);
  }

  let count = 0;
  function backtrack(path) {
    if (path.length === nums.length) {
      console.log(path);
      count += 1;
      return;
    }

    for (const num of freq.keys()) {
      const remainingCopiesOfNumToUse = freq.get(num);

      if (remainingCopiesOfNumToUse === 0) continue;// used all copies of this num? skip

      freq.set(num, remainingCopiesOfNumToUse - 1);
      path.push(num);

      backtrack(path); // curr path, iterate over remaining duplicates of the current number

      path.pop();
      freq.set(num, remainingCopiesOfNumToUse);
    }
  }

  backtrack([]);
  return count;
}
// arr = [1,2,1], n = 3, result = 3!/2! = 6/2 = 3 different paths
const permuteUniqueArray = [1, 2, 1]
console.log('\npermuteUniqueArray', permuteUniqueArray)
console.log("permuteUnique count:", permuteUnique(permuteUniqueArray));

// not yet
// 4) pick p items from k where order does not matter, just the selection, formula: k!/(p!*(k-p)!)
function combinationsWithoutRepetition() {

}
// ....
const combinationsWithoutRepetitionArray = [1, 2, 3]
// console.log('\ncombinationsWithoutRepetitionArray', combinationsWithoutRepetitionArray)
// console.log("combinationsWithoutRepetition count: ", combinationsWithoutRepetition(combinationsWithoutRepetitionArray, [], 0, 0));

// ---------------------------------------------------------------
// For the problems, you don't have to memorize names or formulas, just understand the logic and the multiplication principle

// --> 5 distinct car colors, how many ways to arrange them? n!
// - first position: n choices, second: n-1, ..., last: 1 -> n * (n-1) * ... * 1 = n!
// - 5! = 120 distinct car positions based on color

// --> [ABCAA] 5 letters, 3 are equal; number of DISTINCT arrangements?
// - total cases (if all treated as different letters): 5!
// - repeated arrangements among the 3 equal A's: 3! (these are identical swaps counted multiple times)
// - so we divide total cases by repeated cases to remove duplicate counting
// - result = 5! / 3! = 20 different configurations

// idea: think position by position (multiplication), then correct overcount (with division)

const rl = require('readline').createInterface({ input: process.stdin, output: process.stdout });
rl.question('\nArray size to test or 0 to exit: ', n => {
  const arr = Array.from({ length: +n }, (_, i) => i + 1);

  const start = performance.now();
  permutationDistinctElements(arr, [], 0);
  const t1 = performance.now();

  console.log('permutationDistinctElements:', (t1 - start), 'ms');

  rl.close();
});