// 32-bit addresses
// review all TODO:
// TODO: implement random replacement policy
// TODO: still not working for 2-way, 4-way, or fully associative
// TODO: review the professor's PDF to check if anything else is missing
import fs from "fs";
import { fileURLToPath } from "node:url";
import path from "path";

// import.meta.url --> url of curr file
// fileURLToPath --> understandable OS path (windows or unix)
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const NUM_OF_SETS = Number(process.argv[2]);
const BLOCK_SIZE = Number(process.argv[3]);
const ASSOCIATIVITY = Number(process.argv[4]);
const IS_SPLITTED = process.argv[5] === "true";

console.log("\n-------");
console.log("NUM_OF_SETS", NUM_OF_SETS);
console.log("BLOCK_SIZE", BLOCK_SIZE);
console.log("ASSOCIATIVITY", ASSOCIATIVITY);
console.log("IS_SPLITTED", IS_SPLITTED);
console.log("-------\n");

const hasInvalidNumberParam =
  Number.isNaN(NUM_OF_SETS) ||
  Number.isNaN(BLOCK_SIZE) ||
  Number.isNaN(ASSOCIATIVITY) ||
  NUM_OF_SETS <= 0 ||
  BLOCK_SIZE <= 0 ||
  ASSOCIATIVITY <= 0;

const isInvalidSplitParam = !["true", "false"].includes(process.argv[5]);
if (hasInvalidNumberParam || isInvalidSplitParam) {
  console.log("hasInvalidNumberParam", hasInvalidNumberParam);
  console.log("isInvalidSplitParam", isInvalidSplitParam);
  throw new Error(
    "received invalid params -- check debug flags above and fix your sent params"
  );
}

const OFFSET_BITS_AMOUNT = Math.log2(BLOCK_SIZE);
const INDEX_BITS_AMOUNT = Math.log2(NUM_OF_SETS);

function CacheBuilder({ cacheAccesses }) {
  console.log("cacheAccesses", cacheAccesses);
  let missCount = 0;
  let hitCount = 0;

  const validBitArr = new Array(NUM_OF_SETS).fill(0);
  const tagsArr = new Array(NUM_OF_SETS).fill(0);

  cacheAccesses.forEach((data) => {
    // TODO: review these calcs again -- debug
    const index = NUM_OF_SETS === 1 ? 0 : (data.address >> OFFSET_BITS_AMOUNT) & ((1 << INDEX_BITS_AMOUNT) - 1);
    const tag = data.address >> (OFFSET_BITS_AMOUNT + INDEX_BITS_AMOUNT);

    if (validBitArr[index] == 0) {
      missCount++;
      validBitArr[index] = 1;
      tagsArr[index] = tag;
    } else {
      if (tagsArr[index] == tag) {
        hitCount++;
      } else {
        missCount++;
        validBitArr[index] = 1;
        tagsArr[index] = tag;
      }
    }
  });

  return {
    totalAccesses: cacheAccesses.length,
    hitCount,
    hitRatio: ((hitCount / cacheAccesses.length) * 100).toFixed(2) + "%",
    missCount,
    missRatio: ((missCount / cacheAccesses.length) * 100).toFixed(2) + "%",
    validBitArr,
    tagsArr,
  };
}

function getFormattedInput() {
  // not best way - putting all input.txt into RAM - but we don't care for it here.
  // instead use a stream to process in chunks
  const inputFileText = fs.readFileSync(
    path.join(__dirname, "input.txt"),
    "utf-8"
  );
  const formattedInput = inputFileText
    .split("\n")
    .filter((val) => !!val)
    .map((val) => {
      const splittedLine = val.split(",");
      const address = Number(splittedLine[0].trim());

      if (Number.isNaN(address)) {
        throw new Error("received invalid address on input.txt - only numbers");
      }
      const addressType = splittedLine[1].trim();
      if (
        IS_SPLITTED &&
        (!splittedLine[1] || !["i", "d"].includes(addressType))
      ) {
        throw new Error(
          "splitted cache missing address type or received an invalid type (valid types: i or d) on input.txt file"
        );
      }

      return {
        address,
        type: IS_SPLITTED ? addressType : null, // i = instruction, d = data or NULL if not splitted
      };
    });
  return formattedInput;
}
const formattedInput = getFormattedInput();
if (!(formattedInput && formattedInput.length)) {
  throw new Error(
    "Something went wrong with input file -- check if the format is valid"
  );
}

function showReport({ currCache, prefix }) {
  console.log(`\n---${prefix}----`);
  console.log("totalAccesses", currCache.totalAccesses);
  console.log("hitCount", currCache.hitCount);
  console.log("hitRatio", currCache.hitRatio);
  console.log("missCount", currCache.missCount);
  console.log("missRatio", currCache.missRatio);
  // just to debug if needed
  // console.log('validBitArr', currCache.validBitArr)
  // console.log('tagsArr', currCache.tagsArr)
  console.log("-------\n");
}

if (IS_SPLITTED) {
  const splittedCaches = formattedInput.reduce(
    (prev, curr) => {
      const instructionAddresses = prev.instructionAddresses;
      const dataAddresses = prev.dataAddresses;
      curr.type === "i"
        ? instructionAddresses.push(curr)
        : dataAddresses.push(curr);
      return {
        instructionAddresses,
        dataAddresses,
      };
    },
    { instructionAddresses: [], dataAddresses: [] }
  );

  const DataCache = CacheBuilder({
    cacheAccesses: splittedCaches.dataAddresses,
  });
  const InstructionsCache = CacheBuilder({
    cacheAccesses: splittedCaches.instructionAddresses,
  });
  showReport({ currCache: DataCache, prefix: "DataCache" });
  showReport({ currCache: InstructionsCache, prefix: "InstructionsCache" });
} else {
  const MainCache = CacheBuilder({ cacheAccesses: formattedInput });
  showReport({ currCache: MainCache, prefix: "MainCache" });
}
