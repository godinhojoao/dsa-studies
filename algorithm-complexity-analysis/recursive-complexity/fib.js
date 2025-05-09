// Fib sequence: 0, 1, 1, 2, 3, 5, 8, 13...
// next = prev + curr
// item 0 = 0
// item 1 = 1
// item 2 = 1
// item 3 = 2
// item 4 = 3
// item 5 = 5

// iterative
// Time complexity O(n)
// Space complexity O(1)
// Auxiliary space complexity O(1) -> no extra memory allocated
function iterativeFib(n) {
  // iterativeFib receives n >= 0, the index of the Fibonacci sequence, and returns the corresponding Fibonacci number.
  if (n == 0 || n == 1) { return n };
  let prev = 0;
  let curr = 1;
  for (let i = 2; i <= n; i++) { // n-2 iterations but we drop constants so O(n)
    const temp = curr;
    curr = prev + curr;
    prev = temp;
  }
  return curr;
}
console.log('iterativeFib(5)', iterativeFib(5)) // 5

// recursive
// Time complexity O(2^n) -> branches ^ depth;
// Space complexity O(n) -> Biggest number of items on the stack at same time
// Auxiliary space complexity O(1) -> no extra memory allocated
function recursiveFib(n) {
  // recursiveFib receives n, the index of the Fibonacci sequence, and returns the corresponding Fibonacci number.
  if (n == 0 || n == 1) { return n }
  return recursiveFib(n - 1) + recursiveFib(n - 2);
}
console.log('recursiveFib(5)', recursiveFib(5)) // 5

/*

## Investigating Time Complexity of recursiveFib(n)
recursiveFib(5) calls recursiveFib(4) and recursiveFib(3)
recursiveFib(4) calls recursiveFib(3) and recursiveFib(2)s
recursiveFib(3) calls recursiveFib(2) and recursiveFib(1)
recursiveFib(2) calls recursiveFib(1) and recursiveFib(0)
recursiveFib(1) and recursiveFib(0) returns directly

- For each call, we make two more (except for 1 and 0), branching exponentially.
- Because of that this implementation is O(2^n) time complexity.
  - branch^depth, where depth is n.
  - It will actually run a bit less than 2^n iterations because of 0 and 1, but as we drop constants it is 2^n.

See, we've repeated calls that were not saved. This redundancy could be solved using a more advanced topic called dynamic programming.
- but it's not our purpose for now, just important to notice.

To understand check:
  - the image tree-recursive-fib.png

## Investigating Space Complexity of recursiveFib(n)
- First of all you need to understand the call stack.
- In which when a function is called, it is added to the stack. It is removed from the stack when it returns.
- Example:
  - stack: {}
  - fib(3) called
  - stack: { fib(3) }
  - fib(3) calls fib(2)
  - stack: { fib(3), fib(2) }
  - fib(2) calls fib(1)
  - stack: { fib(3), fib(2), fib(1) }
  - fib(1) returns
  - stack: { fib(3), fib(2) }
  - fib(2) calls fib(0)
  - stack: { fib(3), fib(2), fib(0) }
  - fib(0) returns
  - stack: { fib(3), fib(2) }
  - fib(2) returns
  - stack: { fib(3) }
  - fib(3) calls fib(1)
  - stack: { fib(3), fib(1) }
  - fib(1) returns
  - stack: { fib(3) }
  - fib(3) returns
  - stack: {}
- Notice that the maximum number of function calls on the stack at the same time was 3.
  - Since the input was 3, the maximum call stack depth reached was 3.
  - Consequently, the space complexity is O(n), where n is the input value.
- Maximum depth of the call stack = Space complexity
  - fib(3) -> fib(2) -> fib(1).

Useful resources:
- https://jarednielsen.com/big-o-recursive-time-complexity/
- https://jarednielsen.com/big-o-recursive-space-complexity/
*/
