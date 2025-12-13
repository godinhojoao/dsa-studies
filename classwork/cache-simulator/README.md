## To run:

- 1. Install Nodejs v22 or run it using a Nodejs v22 Docker container.
- 2. `node cache-simulator.js nsets:bsize:assoc input_file` (unified cache)
- 3. `node cache-simulator.js nsets_iL1:bsize_iL1:assoc_iL1 nsets_dL1:bsize_dL1:assoc_dL1 input_file` (split cache)

## Input file format:

- Note: if it's unified cache the type param will be ignored.

```txt
## Format:
address
type ## 0 = instruction 1 = data

## Example of file:
100
1
101
0
```

## Running with configurations (all with block size = 4 bytes and cache size = 4096 bytes):

- **Direct mapped** (4096 bytes: 1024 sets x 4 bytes x 1-way)
  - `node cache-simulator.js 1024:4:1 input1-ignore.txt`
- **2-way set associative** (4096 bytes: 512 sets x 4 bytes x 2-way)
  - `node cache-simulator.js 512:4:2 input1-ignore.txt`
- **4-way set associative** (4096 bytes: 256 sets x 4 bytes x 4-way)
  - `node cache-simulator.js 256:4:4 input1-ignore.txt`
- **Fully associative** (4096 bytes: 128 sets x 4 bytes x 8-way)
  - `node cache-simulator.js 128:4:8 input1-ignore.txt`

## Split cache configurations using the same size for iL1 and dL1 (each with block size = 4 bytes and cache size = 4096 bytes):

- **Direct mapped (splitted)** (4096 bytes each: 1024 sets x 4 bytes x 1-way)
  - `node cache-simulator.js 1024:4:1 1024:4:1 input1-ignore.txt`
- **2-way set associative (splitted)** (4096 bytes each: 512 sets x 4 bytes x 2-way)
  - `node cache-simulator.js 512:4:2 512:4:2 input1-ignore.txt`
- **4-way set associative (splitted)** (4096 bytes each: 256 sets x 4 bytes x 4-way)
  - `node cache-simulator.js 256:4:4 256:4:4 input1-ignore.txt`
- **Fully associative (splitted)** (4096 bytes each: 128 sets x 4 bytes x 8-way)
  - `node cache-simulator.js 128:4:8 128:4:8 input1-ignore.txt`

## OBS:

- if user doesn't input anything it will use default 1024:4:1 not splitted and input.txt as input file.
