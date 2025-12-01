## To run:

- 1. Install Nodejs v22 or run it using a Nodejs v22 Docker container.
- 2. `node cache-simulator.js num_of_sets block_size associativity is_splitted`
  - is_splitted: "true" or "false"

## Input file format:

- Note: if it's not a splitted cache the i or d param will be ignored, you don't need to send it, but you can let it as it was.

```txt
## Format:
address, data
address, instruction

## Example of file:
10, i
11, d
```

## Running with different configurations:

- (All examples are non-splitted, to make them splitted just change the last parameter.)

- **Direct mapped**
  - `node cache-simulator.js 16 32 1 false`
- **2-way Set associative**
  - `node cache-simulator.js 4 32 2 false`
- **4-way**
  - `node cache-simulator.js 8 32 4 false`
- **Fully associative cache**
  - `node cache-simulator.js 1 32 8 false`
