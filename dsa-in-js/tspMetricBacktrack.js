// optimal solution for TSP Metric
// just for study, don't use it since it's too slow (NP-hard).
// It's better to develop an approximate algorithm for this problem.
// TSP metric properties:
// 1. d(x,y) >= 0 (not negative)
// 2. d(x,y) = d(y,x) (symmetric -> undirected)
// 3. d(x,y) + d(y,z) >= d(x,z) (triangle inequality)

const adjMatrix = [
  [0, 29, 20, 21, 16, 31, 100, 12, 4, 31, 18],
  [29, 0, 15, 29, 28, 40, 72, 21, 29, 41, 12],
  [20, 15, 0, 15, 14, 25, 81, 9, 23, 27, 13],
  [21, 29, 15, 0, 4, 12, 92, 12, 25, 13, 25],
  [16, 28, 14, 4, 0, 16, 94, 9, 20, 16, 22],
  [31, 40, 25, 12, 16, 0, 95, 24, 36, 3, 37],
  [100, 72, 81, 92, 94, 95, 0, 90, 101, 99, 84],
  [12, 21, 9, 12, 9, 24, 90, 0, 15, 25, 13],
  [4, 29, 23, 25, 20, 36, 101, 15, 0, 35, 18],
  [31, 41, 27, 13, 16, 3, 99, 25, 35, 0, 38],
  [18, 12, 13, 25, 22, 37, 84, 13, 18, 38, 0]
];
const vertices = 11;

function getDistance(u, v) {
  return adjMatrix[u][v];
}
function getLastUsedNodeIndex(visitedNodesIndexes) {
  return visitedNodesIndexes[visitedNodesIndexes.length - 1];
}

function backtrackTSPMetric({ adjMatrix, bestCost = Infinity, currCost = 0, visitedNodesIndexes = [0] }) {
  if (visitedNodesIndexes.length == vertices) {
    currCost += getDistance(getLastUsedNodeIndex(visitedNodesIndexes), visitedNodesIndexes[0]); // last used node return to first one (TSP ham cycle)
    bestCost = currCost < bestCost ? currCost : bestCost;
    return bestCost;
  }

  for (let i = 1; i < vertices; i++) {
    // Avoid mirroring (only possible because it's symetric/undirected)
    // 01230 == 03210 (path[1] = 1, path[1] = 3) we need to choose which to count to avoid counting both
    // lastButOne < path[i]? accept : skip (if want to accept first side of mirror)
    // lastButOne < path[1]? accept : skip (if want to accept second side of mirror)
    const isLastButOneOfPath = visitedNodesIndexes.length === vertices - 1;
    if (isLastButOneOfPath && i > visitedNodesIndexes[1]) {
      continue;
    }

    if (!visitedNodesIndexes.includes(i)) {
      const currAddedDistance = getDistance(getLastUsedNodeIndex(visitedNodesIndexes), i);;
      currCost += currAddedDistance;
      visitedNodesIndexes.push(i);
      bestCost = backtrackTSPMetric({ adjMatrix, bestCost, currCost, visitedNodesIndexes });
      visitedNodesIndexes.pop();
      currCost -= currAddedDistance;
    }
  }
  return bestCost;
}

const t0 = performance.now();
const bestcost = backtrackTSPMetric({ adjMatrix })
console.log('bestcost', bestcost)
const t1 = performance.now();
console.log(`time: ${t1 - t0} ms.`);
