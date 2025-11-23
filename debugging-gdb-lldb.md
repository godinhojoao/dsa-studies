# Introduction to GNU, GDB, ELF, and LLDB

## What is GNU?

- A free operating system often used with the Linux kernel (GNU/Linux). GNU provides compilers, libraries, shells, utilities and other needed software to run a computer. While Linux manages hardware, memory, and processes.

## What is GDB ?

- **GNU Debugger**, A command-line tool used to debug programs by inspecting them as they run, analyze and modify their state, add breakpoints, and more.
- If a language/compiler outputs a Linux executable in ELF format and you include debug symbols, GDB can debug it.
  - **Executable and Linkable Format (ELF)** is the standard binary file format on Linux. It defines how compiled code and data are stored so the OS can load and run programs.
  - **Object file**: Partially compiled code with unresolved references. Containing:
    - **Machine code** for one source file: Your code compiled to CPU instructions – shown as mnemonics for humans, but the computer actually uses only 0’s and 1’s.
    - **Symbol Definitions**: (functions/variables it provides) _"hey linker, I'm declaring these variables"_
    - **Symbol References**: (functions/variables it needs but doesn't have) _"hey linker, I need these variables"_
    - **Relocation Info**: (places that need addresses fixed later by the linker)
      ```c
        void foo() { ... }
        void bar() { foo(); } // relocation info says: "Hey linker, in bar() at this spot fix the address of foo()"
      ```
  - **Shared library**: A compiled library (`.so` file) containing reusable code that programs can load at runtime, like OS functions (file system, math, etc...)
  - **Executable**: A fully linked program in ELF format that combines all object files and libraries, with all addresses resolved, ready for the OS to run.
  - **The Link process**: The step where the linker takes the object files and libraries, resolves symbol references, fixes addresses (relocation), and produces the final executable or shared library.

## 1. Compile your code with the necessary flag

- `gcc test.c -g -O0 -o test`
- `-g`: Generate source-level debug information.
- `-O0`: Disable optimizations.
- This generates an object file (`.o` file), not a final executable. It's not yet linked with necessary libraries or other object files to form a complete runnable program.

## 2. Run your executable with GDB

- `gdb ./test`: Start gdb with your compiled object file

## 3. Add and remove breakpoints

- **Adding breakpoint**:
  - `break line number`: break 30
  - `break func name`: break main
- **Clearing breakpoint**:
  - `clear line number`: clear 30
  - `clear func name`: clear main

## 4. More basic commands (first run `gdb` to start )

- `help`: list of gdb commands.
- `run`: Run your program
- `watch variable_name`: Stop execution when variable changes.
- `print variable_name`: Print the value of the variable.
- `set variable x = 42`: Change variable value.
- `step`: Enters the next function and stops at first line **inside the function**.
- `next`: Run next line, if a function, it runs the entire function but do not enter on it, stops at the first line **after the function**.
- `bt`: show the callstack (backtrace).
- `list 1`: show code lines around line 1.
- `quit`: to exit.

## If you are using MacOS, use LLDB instead of GDB;

- LLDB is similar to GDB, a debugger, but designed for the LLVM compiler toolchain.

- [First compile your code the same way as before](#1-compile-your-code-with-the-necessary-flag)

- **Commands**:

  - `help`: list lldb commands
  - `lldb ./executable`: start debugging your executable with lldb
  - `run`: run your program inside lldb
- **Breakpoint management**:
  - `b 4`: add breakpoint at line 4
  - `b main.c:30`: add breakpoint at line 30
  - `b main`: add breakpoint at function main
  - `br l`: list all breakpoints and its ids
  - `br del id`: delete breakpoint 1
  - `br del`: delete all breakpoints
  - `watchpoint set variable var_name`: stop when variable changes
- **Frame**: A level in the call stack. Level 0 is the top of the stack, showing the current function, variables, and more. (`fr` shortcut)
    - `frame info`: detailed information about current frame. (`fr info` works too)
    - `fr var`: show all local variables
    - `fr var var_name`: show specific variable
      - `frame v` shortcut
    - `fr select 1`: change the current frame for debugging variables and more. (`frame s 1` shortcut)
- **expression var_name**: evaluate expression and print output (`e something` shortcut)
    - `e *myPointer` -> dereference myPointer and show value
    - `e x + y` -> prints x+y
    - `e foo(10)` -> run and print output of foo(10)
    - `e x = 42` -> change variable value
  - `step`: step into next line (enter functions) `s` shortcut
  - `next`: step over next line `n` shortcut
  - `finish`: run until function returns
  - `continue` or `c`: resume execution, run until next breakpoint, watchpoint or the program ends
  - `bt`: show the callstack (backtrace) with all its frames (0 is the top), others are the callers.
- **List**: it will show 10 lines, you can press enter to see more 10 lines.
    - `list`: show code lines around current line.
    - `list 1`: show code lines around line 1.
    - `list main`: show code lines around function main.
  - `quit`: exit lldb

- **Short Tips**:
  - Use `e *myPtr` to see dereferenced pointer values
  - Use `fr s 0` to do a frame select 0 and see current and around lines you've stopped if needed.
  - Use `br del` to delete all breakpoints
  - **If you get a segfault**, use `bt` to see the stack frames.
    - Then select the frame **before** the segfault with `fr s 1`, and use `fr v` to inspect variables `e expresion` to run expressions.
      - You can continue debugging from there.
      - It’s like traveling back in time: you can evaluate expressions, check variables, and continue debugging normally.
      - **Note:** breakpoints set after the crash will not work, since the segfault has already occurred.
  - If you see something like `(char *) $7 = 0x0000000000000000` this is a **null pointer**.
    - Notice something cool: we have **16 zeros**. Since it is hexadecimal, each digit represents 16 values (2^4), which means each digit is 4 bits.
    - 16 digits \* 4 bits = 64 bits, which matches the architecture of my computer (`arm64`).

## For unix terminal users: Create a shortcut like this one in `.zshrc`

```bash
# you can run: gcc-debug ./filename.c
gcc-debug() {
  set -x            # show commands tracing
  gcc "$1" -g -O0 -o test && lldb ./test
  set +x            # turn off tracing
}
```

## Use this simple code to practice the commands above:

```c
#include <stdlib.h>
int main() {
  int i = 10;
  int j = 5;

  int* p = NULL;
  *p = 10;  // segfault, null pointer dereference

  return 0;
}
```

## References:

- [Executable and Linkable Format - ELF](https://wiki.osdev.org/ELF)
- [GDB](https://ftp.gnu.org/old-gnu/Manuals/gdb/html_chapter/gdb_toc.html)
- [LLVM Debugger - LLDB](https://lldb.llvm.org/)
