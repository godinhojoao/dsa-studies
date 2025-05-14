# Algorithm Complexity Analysis PART I - Big O

- This is part of an article series on **Algorithm Complexity Analysis**.
  - In **Part I**, we cover key topics related to **Big O notation**.

## Table of Contents

- [What is Big O?](#what-is-big-o)
- [What does Asymptotic and Asymptotic Notation mean?](#what-does-asymptotic-and-asymptotic-notation-mean)
- [Time Complexity](#time-complexity)
  - [1. Constant Time - O(1)](#1-constant-time---o1)
  - [2. Linear Time - O(n)](#2-linear-time---on)
  - [3. Quadratic Time - O(n^2)](#3-quadratic-time---on2)
- [Space Complexity](#space-complexity)
  - [1. Constant Space - O(1)](#1-constant-space---o1)
  - [2. Linear Space - O(n)](#2-linear-space---on)
  - [3. Quadratic Space - O(n^2)](#3-quadratic-space---on2)
- [Determining the Complexity of a Recursive Algorithm](#determining-the-complexity-of-a-recursive-algorithm)
- [Big-O Complexity Chart](#big-o-complexity-chart)
- [Four rules of Big O](#four-rules-of-big-o)
  - [1. Worst Case](#1-worst-case)
  - [2. Drop Constants](#2-drop-constants)
  - [3. Different Inputs => Different Variables](#3-different-inputs--different-variables)
  - [4. Drop Non-Dominant Terms](#4-drop-non-dominant-terms)
- [Why do we consider exchanging space complexity in favor of time complexity worth it?](#why-do-we-consider-exchanging-space-complexity-in-favor-of-time-complexity-worth-it)
- [Pros of Big O](#pros-of-big-o)
- [Cons of Big O](#cons-of-big-o)
- [References](#references)

## What does Asymptotic and Asymptotic Notation mean?

- **Problems with measuring time and space by just running the algorithm**:
  - Results vary across different computers.
  - Background processes and daemons can affect performance even on the same computer.
  - Without the computing power of companies like Google, we rely on asymptotic notation to understand algorithm efficiency with large inputs.
  - `That’s why we use Asymptotic notation`; to consistently evaluate time and space complexity, machine-independently.
- `Asymptotic`:
  - While having a background in calculus is helpful, it's not strictly necessary to understand asymptotic analysis.
  - Asymptotic analysis studies **how a function behaves as its input approaches infinity**. For example, consider a function `f(x)`; as x increases more and more, how does the function behave? [Watch this](https://www.youtube.com/watch?v=myZKhztFhzE&ab_channel=BackToBackSWE)
- `Asymptotic notation`:
  - Like calculus uses limits to analyze behavior at extremes, asymptotic notation uses:
    - `Big O` for the upper limit (worst-case).
    - `Big Omega` for the lower limit (best-case).
    - `Big Theta` for the tight bound (typically used for average-case or general characterization).
- We normally refer to **Algorithmic Complexity** with expressions like: `factorial time`, `quadratic time`, `linear time`, and so on.
  - This means we are **not describing a specific graph**, but rather **classifying the growth behavior** of the algorithm's runtime.

## What is Big O?

- Big O is a notation used to describe an algorithm's time complexity and space complexity in terms of the input size.
  - How the execution time (or memory usage) of an algorithm increases as the size of the input grows.
  - Big O isn't primarily about performance; it's about scalability;

## Time Complexity

- **What is it?**
  - The amount of time an algorithm takes to run as a function of the input size.
  - It gives an estimate of the efficiency of an algorithm and helps us understand how it will scale as the input grows.
- **How to measure it?**
  - Count the number of basic operations (like comparisons or loops) an algorithm performs relative to the input size.

### 1. Constant Time - `O(1)`

- No matter the input size, the algorithm performs a fixed number of operations.

```js
function getFirstElement(arr) {
  return arr[0]; // Only one operation, regardless of array size
}
```

### 2. Linear Time - `O(n)`

- The algorithm performs operations proportional to the input size.

```js
function printAllElements(arr) {
  for (let i = 0; i < arr.length; i++) {
    console.log(arr[i]); // Loops through all elements
  }
}
```

### 3. Quadratic Time - `O(n^2)`

- The algorithm performs operations proportional to the square of the input size, often seen in nested loops.

```js
function quadraticTime(arr) {
  for (let i = 0; i < arr.length; i++) {
    for (let j = 0; j < arr.length; j++) {
      console.log(arr[i], arr[j]); // Loops inside loops
    }
  }
}
```

## Space Complexity

- **What is it?**
  - Space complexity measures the amount of memory an algorithm uses relative to the input size.
  - It helps evaluate the efficiency of memory usage as the input size increases.
- **How to measure it?**
  - Count the amount of memory required for variables, data structures, and recursive calls as the input size grows.
- A common mistake when analyzing algorithms is confusing `Auxiliary Space` with `Space Complexity`; but they’re not the same:
  - `Auxiliary Space`: Extra memory used by the algorithm, excluding the input/output.
  - `Space Complexity`: Total memory used, including input, output, and auxiliary space.
- Below we will have some examples of `Auxiliary Space`.

### 1. Constant Space - `O(1)`

- No matter the size of the input, the algorithm uses a constant amount of auxiliary space.

```js
function constantSpace(array) {
  let sum = 0; // One variable sum
  for (let i = 0; i < array.length; i++) {
    // One variable i
    sum += array[i];
  }
  return sum;
}
```

### 2. Linear Space - `O(n)`

- The algorithm spends proportional auxiliary space to the input size.

```js
function linearSpace(array) {
  let result = []; // new array size n
  for (let i = 0; i < array.length; i++) {
    result.push(array[i] * 2); // storing at this array size n
  }
  return result;
}
```

### 3. Quadratic Space - `O(n^2)`

- The algorithm spends proportional auxiliary space to the square of the input size.

```js
function quadraticSpace(n) {
  let matrix = []; // new 2d array
  for (let i = 0; i < n; i++) {
    matrix[i] = []; // new row for each item (rows size n)
    for (let j = 0; j < n; j++) {
      matrix[i][j] = i + j; // populate the columns of each row (columns size n)
    }
  }
  return matrix;
}
```
## Determining the Complexity of a Recursive Algorithm

- In this section, we will see how to determine **space and time complexity** for **recursive algorithms**, but we will not dive deeply into Recurrence Relations and the Master Theorem.
- We’ll use the classic example: recursive Fibonacci.

```js
function recursiveFib(n) {
  if (n == 0 || n == 1) return n;
  return recursiveFib(n - 1) + recursiveFib(n - 2);
}
```

- **Time Complexity: O(2^n)**
- Each call branches into two others, forming a binary tree with depth `n`.
- This exponential growth leads to ~2^n calls.
  - It's not exactly 2^n, but we drop constants and consider O(2^n).
- See the tree below to understand how the calls branch:
  ![Recursive Fibonacci Tree](https://i.ibb.co/kgjmzKxR/tree-recursive-fib.png)

- **Space Complexity: O(n)**

  - This comes from how the **call stack** works during recursion.
  - When a function calls itself, it’s paused and added to a stack until the recursive call finishes.
  - In the worst case (e.g., leftmost path of the call tree), the stack can grow to depth `n` before the functions finish and return.
  - So, the maximum number of active function calls at once is proportional to `n`.

- For better understanding, look:
  - [Fib C example](./recursive-complexity/fib.c)
  - [Fib JS example](./recursive-complexity/fib.js)

## Big-O Complexity Chart
  ![Big-O Complexity Chart](https://i.ibb.co/dXDFhNr/Screenshot-2025-05-08-at-22-32-32.png)
  - https://www.bigocheatsheet.com/

## Four rules of Big O

### 1. Worst Case

- If we got Jack as the first user, we would have O(1), but this case doesn't matter for Big O notation.
- Big O cares only about the **worst case**, and this is **O(n)**.

  ```js
  function findJack(users) {
    for (let i = 0; i < users.length; i++) {
      const user = users[i];
      if (user.name === "Jack") {
        console.log(user);
        break;
      }
    }
  }
  ```

### 2. Drop Constants

- What matters is the asymptotic behavior, which means that we analyze the growth of an algorithm as the input size `n` approaches infinity. And because of that, constants become negligible compared to how the function grows.
  - O(2n), O(100n) and O(10000n) all grow linearly as `n` grows.
  - The constant factors don't affect the shape of the growth curve, just the initial height.
- It's also good because it simplifies comparisons, for example, it's easy to see that `O(n log n)` is better than `O(n^2)`.

  ```js
  // - This is not O(2n), because Big O measures growth patterns, not exact performance.
  // - So this is O(n).
  function minMax(array) {
    let min = null,
      max = null;
    array.forEach((e) => {
      min = MIN(e, min);
    });
    array.forEach((e) => {
      max = MAX(e, max);
    });
  }
  ```

### 3. Different Inputs => Different Variables

- This is **not** O(n^2) because it's not the same input.
- It is **O(a \* b)**.

  ```js
  function intersection(arrayA, arrayB) {
    let count = 0;
    arrayA.forEach((a) => {
      arrayB.forEach((b) => {
        if (a === b) {
          count += 1;
        }
      });
    });
  }
  ```

### 4. Drop Non-Dominant Terms

- We do this for the same reason we drop constants: to focus on the worst-case growth rate.
- Only the fastest-growing term affects Big O.

  ```js
  // - This is not O(n + n^2), because we drop the non-dominant term.
  // - So it's O(n^2).
  function minMax(array) {
    // O(n)
    let min = null;
    let max = null;
    array.forEach((e) => {
      min = MIN(e, min);
    });
    console.log(min);

    // O(n^2)
    array.forEach((e1) => {
      array.forEach((e2) => {
        console.log(e1, e2);
      });
    });
  }
  ```

## Why do we consider exchanging space complexity in favor of time complexity worth it?

- **Space complexity** = how much memory (RAM, disk, etc.) a program uses
- **Time complexity** = how long the program takes to run
- This trade-off is often considered a good deal because, in many cases, **we can buy more memory, but we can't buy more time**.
  - For example, in the easy Leetcode problem "Two Sum", we can trade **Auxiliary Space from O(1) to O(n)** in order to **improve time complexity from O(n^2) to O(n)** by using a **hash table**.

## Pros of Big O

- **1 - Useful for comparing Algorithms thanks to Asymptotic Analysis**
  - It provides a high-level view of the algorithm efficiency which makes easy to see that O(n) is much faster than O(n^2) for larger inputs.
- **2 - Helps us to understand the trade-off between space and time.**
  - With practice, we notice that in many cases, as mentioned before, we can make a trade-off between space and time, depending on which one we need more.
- **3 - It's theoretical and can be generalized**
  - If we tried to consider each hardware, OS, and other factors to measure our code's efficiency, it would never be precise, as it can vary depending on many "external" conditions.
  - It's similar to how physicists make assumptions 'in a vacuum' to focus on fundamental principles, ignoring external influences like air resistance that would complicate the model.

## Cons of Big O

- The problem is the **misuse of Big O notation**; not Big O itself.
- These issues arise not from Big O itself, but from relying on it exclusively without considering other complexity analysis notations and average or best case scenarios.

- **1 - Focuses on worst case**
  - In some scenarios, using Big O is appropriate; for example, in a time-critical system running on an airplane flying over a mountain. If the algorithm is only designed for average or best-case scenarios rather than the worst case, the consequences could be serious.
  - However, it's important to recognize that in some cases, analyzing the average or best case may be more useful, depending on the specific use case of the algorithm.
- **2 - Ignores constants**
  - Only shows asymptotic growth; O(1000n) and O(n) considered same thing.
  - This is good in general but can mislead in real-world scenario with many constant factors.
  - Read for more details: [When do Constants Matter?](https://pages.cs.wisc.edu/~vernon/cs367/notes/3.COMPLEXITY.html#:~:text=Recall%20that%20when%20we%20use,those%20terms%20don't%20matter.)
    - If you will work with small inputs it's important to care about constants.

## References

- https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1176/handouts/midterm/5-BigO.pdf
- https://pages.cs.wisc.edu/~vernon/cs367/notes/3.COMPLEXITY.html
- https://www.bigocheatsheet.com/
- https://dev.to/coderjay06/four-rules-for-big-o-1915
- https://www.geeksforgeeks.org/analysis-algorithms-big-o-analysis/
- https://www.youtube.com/watch?v=myZKhztFhzE&ab_channel=BackToBackSWE
- https://www.geeksforgeeks.org/g-fact-86/
- https://jarednielsen.com/big-o-recursive-time-complexity/
- https://jarednielsen.com/big-o-recursive-space-complexity/
- https://skilled.dev/course/big-o-time-and-space-complexity
