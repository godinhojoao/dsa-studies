# João Godinho M2

## Create an agenda with the following menu:

1. - Add Person (Name, Age, Email)
2. - Remove Person
3. - Search Person
4. - List All
5. - Exit

- The challenge is to not create any variables, but instead use a single memory buffer (void \*pBuffer).
  With this, you'll have to do the job the Operating System usually does for you in organizing memory.

## Rules:

- No variables can be declared throughout the entire program, only pointers.
- All program data must be stored inside pBuffer.
- Not even function parameters can be variables. Only pointers that point inside pBuffer are allowed.
  Examples of what is NOT allowed: int c; char a; int v[10]; void Function(int parameter)
- Structs are also NOT allowed anywhere in the program.

# Run with valgrind: (Dr memory and valgrind are incompatible with macos)

1 - install docker on oficial website
2 - Run on terminal: `docker build -t valgrind-runner . && docker run valgrind-runner`
