import fs from "fs";
import { fileURLToPath } from "node:url";
import path from "path";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// number:number:number
function getCacheConfig(configString) {
  // default config when none is provided: 1024 sets, block size 4, associativity 1
  if (!configString) {
    return {
      numOfSets: 1024,
      blockSize: 4,
      associativity: 1,
    };
  }

  const [numSets, blockSize, associativity] = configString.split(":").map(Number);
  // & bitwise and; compares each bit and returns 1 only if both are 1
  // example: 8 (1000) & 7 (0111) = 0, but 6 (0110) & 5 (0101) = 4
  // log2 here didn't work well because of huge numbers and the float precision issue with Math.log2
  const isPowerOfTwo = (n) => n > 0 && (n & (n - 1)) === 0;

  // block size must be at least 4 bytes since we read 32bit words (4bytes)
  if (!numSets || !blockSize || !associativity) {
    throw new Error("Invalid config");
  }
  if (!isPowerOfTwo(blockSize) || !isPowerOfTwo(numSets) || !isPowerOfTwo(associativity)) {
    throw new Error("Invalid config");
  }

  return { numOfSets: numSets, blockSize: blockSize, associativity: associativity };
}

const IS_SPLITTED = process.argv.length === 5;
const config1 = getCacheConfig(process.argv[2]);
const config2 = IS_SPLITTED ? getCacheConfig(process.argv[3]) : null;
let inputFile = process.argv[IS_SPLITTED ? 4 : 3];
if (!inputFile) {
  inputFile = "input.txt";
}

function readInput(fileName) {
  const filePath = path.isAbsolute(fileName) ? fileName : path.join(__dirname, fileName);
  // binary file logic
  // if (fileName.endsWith('.bin')) {
  // TODO: it isn't working correctly
  // }

  const lines = fs.readFileSync(filePath, "utf-8")
    .split("\n")
    .filter(line => line.trim());

  const result = [];
  // reading two by two i+=2
  for (let i = 0; i < lines.length; i += 2) {
    if (i + 1 < lines.length) {
      result.push({ address: Number(lines[i]), type: Number(lines[i + 1]) });
    }
  }
  return result;
}

class Cache {
  constructor(name, numOfSets, blockSize, associativity) {
    this.name = name;
    this.numOfSets = numOfSets;
    this.associativity = associativity;
    this.blockSize = blockSize;
    this.bitsOffset = Math.log2(blockSize);
    this.bitsIndex = Math.log2(numOfSets);
    this.accesses = 0;
    this.hits = 0;
    this.totalMisses = 0;
    this.compulsoryMiss = 0;
    this.conflictMiss = 0; // sending some capacity misses to conflict misses (to simplify)
    this.cacheSize = numOfSets * blockSize * associativity;
    this.cache = Array(numOfSets) // cache[sets][associativity]
      .fill(null)
      .map(() =>
        Array(associativity)
          .fill(null)
          .map(() => ({
            validBit: false,
            tag: 0,
          }))
      );
  }

  search(address) {
    this.accesses++;

    // >>> shifts bits right and fills left with zeros (treats number as unsigned)
    // index: which set in the cache
    const index = this.bitsIndex === 0
      ? 0
      : (address >>> this.bitsOffset) & ((1 << this.bitsIndex) - 1);

    // tag: upper bits that identify the block
    const tag = address >>> (this.bitsOffset + this.bitsIndex);

    if (this.associativity === 1) {
      return this.searchDirectMapped(index, tag);
    }

    return this.searchSetAssociative(index, tag);
  }

  searchDirectMapped(index, tag) {
    const line = this.cache[index][0];

    if (!line.validBit) {
      this.compulsoryMiss++;
      this.insert(index, tag);
      return;
    }

    if (line.tag === tag) {
      this.hits++;
      return;
    }

    // validBit true but tag not matching = conflict miss
    this.conflictMiss++;
    this.insert(index, tag);
  }

  searchSetAssociative(index, tag) {
    // check for hit in any way
    for (let i = 0; i < this.associativity; i++) {
      // this for is simulating the multiple comparators a cache contains in hardware
      const line = this.cache[index][i];
      if (line.validBit && line.tag === tag) {
        this.hits++;
        return;
      }
    }

    // no hit found, insert randomly
    this.insert(index, tag);
  }

  insert(index, tag) {
    // direct mapped insert the value using only index
    if (this.associativity === 1) {
      this.cache[index][0].validBit = true;
      this.cache[index][0].tag = tag;
      return;
    }

    // if not direct mapped, check if any empty slot exists for miss classification
    let hasEmptySlot = false;
    for (let i = 0; i < this.associativity; i++) {
      if (!this.cache[index][i].validBit) {
        hasEmptySlot = true;
        break;
      }
    }

    // classify miss type
    if (hasEmptySlot) {
      this.compulsoryMiss++;
    } else {
      this.conflictMiss++;
    }

    // insert randomly
    const target = Math.floor(Math.random() * this.associativity);
    this.cache[index][target].validBit = true;
    this.cache[index][target].tag = tag;
  }

  printLog() {
    this.totalMisses = this.compulsoryMiss + this.conflictMiss;

    const hitRatio = this.accesses === 0 ? 0 : this.hits / this.accesses;
    const missRatio = this.accesses === 0 ? 0 : this.totalMisses / this.accesses;

    console.log(`\nCache ${this.name} Report`);
    console.table({
      "Cache size (bytes)": this.cacheSize,
      "Sets": this.numOfSets,
      "Associativity": this.associativity,
      "Block size (bytes)": this.blockSize,
      "Accesses": this.accesses,
      "Hits": this.hits,
      "Misses": this.totalMisses,
      "Compulsory misses": this.compulsoryMiss,
      "Conflict misses": this.conflictMiss,
      "Hit rate": hitRatio.toFixed(6),
      "Miss rate": missRatio.toFixed(6)
    });
  }
}

const input = readInput(inputFile);
if (IS_SPLITTED) {
  const iCache = new Cache("iL1", config1.numOfSets, config1.blockSize, config1.associativity);
  const dCache = new Cache("dL1", config2.numOfSets, config2.blockSize, config2.associativity);
  for (const { address, type } of input) {
    const cache = type === 0 ? iCache : dCache; // 0 = instruction 1 = data
    cache.search(address);
  }
  iCache.printLog();
  dCache.printLog();
} else {
  const cache = new Cache("L1", config1.numOfSets, config1.blockSize, config1.associativity);
  for (const { address } of input) {
    cache.search(address);
  }
  cache.printLog();
}
