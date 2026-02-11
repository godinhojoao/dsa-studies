/*
Deleting the executable after running:
gcc graphs.c -o test && ./test && rm -rf ./test
*/

/*
Undirected and complete graph for cities, version with adjacency matrix and also adjacency list:

Time and Space Complexity:

v = vertices = cities
e = edges = path between cities

Matrix big o notation (worst case):
- addCitiesEdgeMatrix:
  - time: O(v^2) we need to copy all the values to the new matrix
  - space: O(v^2) we realloc an entire new matrix
- removeCityAndAllEdgesMatrix:
  - time: O(v^2) we need to copy all the values to the new matrix
  - space: O(v^2) we realloc an entire new matrix
- getDistanceBetweenCitiesMatrix:
  - time: O(1) because we access by indexes directly
  - space: O(1) dont alloc any new space for this.
- editDistanceBetweenCitiesMatrix:
  - time: O(1) because we access by indexes directly
  - space: O(1) dont alloc any new space for this.

Adjacency list:
  - funcname: O() -> because...
  - funcname: O() -> because...
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct City {
  int id;
  double xPos;
  double yPos;
} typedef City;

struct MatrixGraphController {
  double** matrix;
  int currentDimension;
  int maxCityId;
  int* cityIdToIndex;
  int* indexToCityId;
} typedef MatrixGraphController;

// https://en.wikipedia.org/wiki/Euclidean_distance
double calcCitiesEuclideanDistance(City source, City destination) {
  double distance = sqrt(pow(source.xPos - destination.xPos, 2) + pow(source.yPos - destination.yPos, 2));
  printf("just to debug: source.id: %d to destination.id %d -> distance: %.2f\n", source.id, destination.id, distance);
  return distance;
}

// O(v^2) since matrix is vertices^2
double** reallocMatrix(double** oldMatrix, int oldDimension, int newDimension) {
  double** newMatrix = malloc(sizeof(double*) * newDimension);

  for(int i = 0; i < newDimension; i++) {
    newMatrix[i] = calloc(newDimension, sizeof(double));
  }

  int minDimension = oldDimension < newDimension ? oldDimension : newDimension;

  // copying existent elements
  for(int i = 0; i < minDimension; i++) {
    for(int j = 0; j < minDimension; j++) {
      newMatrix[i][j] = oldMatrix[i][j];
    }
  }

  for(int i = 0; i < oldDimension; i++) {
    free(oldMatrix[i]);
  }
  free(oldMatrix);

  return newMatrix;
}

void addCitiesEdgeMatrix(MatrixGraphController* graph, int srcCityId, int destCityId, double distance) {
  int srcMatrixIdx = graph->cityIdToIndex[srcCityId];
  int destMatrixIdx = graph->cityIdToIndex[destCityId];

  // grow for src if needed
  if(srcMatrixIdx == -1) {
    int oldDimension = graph->currentDimension;

    graph->matrix = reallocMatrix(graph->matrix, oldDimension, oldDimension + 1);
    graph->indexToCityId = realloc(graph->indexToCityId, sizeof(int) * (oldDimension + 1));

    graph->cityIdToIndex[srcCityId] = oldDimension;
    graph->indexToCityId[oldDimension] = srcCityId;

    graph->currentDimension++;
    srcMatrixIdx = oldDimension;

    destMatrixIdx = graph->cityIdToIndex[destCityId];  // to recheck if we still need to grow for dest id
  }

  // grow for dest if needed
  if(destMatrixIdx == -1) {
    int oldDimension = graph->currentDimension;

    graph->matrix = reallocMatrix(graph->matrix, oldDimension, oldDimension + 1);
    graph->indexToCityId = realloc(graph->indexToCityId, sizeof(int) * (oldDimension + 1));

    graph->cityIdToIndex[destCityId] = oldDimension;
    graph->indexToCityId[oldDimension] = destCityId;

    graph->currentDimension++;
    destMatrixIdx = oldDimension;
  }

  // undirected graph here
  graph->matrix[srcMatrixIdx][destMatrixIdx] = distance;
  graph->matrix[destMatrixIdx][srcMatrixIdx] = distance;
}

void removeCityAndAllEdgesMatrix(MatrixGraphController* graph, int sourceCityId) {
  // current
  // 0 [0,1,1]
  // 1 [1,0,1]
  // 2 [1,1,0]
  //
  // after removing node 1 and all its related edges
  // (the representation changes because now idx 2 turns into idx 1) -> we move the ones after the removed 1 step behind
  // 0 [0,1]
  // 1 [1,0]
  int removeIndex = graph->cityIdToIndex[sourceCityId];
  if(removeIndex == -1) {
    return;
  }

  int oldDimension = graph->currentDimension;
  int newDimension = oldDimension - 1;

  // move the rows one step behind starting in the removed index, since we will remove it we can't let a blank space
  for(int i = removeIndex; i < oldDimension - 1; i++) {
    for(int j = 0; j < oldDimension; j++) {
      graph->matrix[i][j] = graph->matrix[i + 1][j];
    }
  }

  // same for columns
  for(int j = removeIndex; j < oldDimension - 1; j++) {
    for(int i = 0; i < oldDimension - 1; i++) {
      graph->matrix[i][j] = graph->matrix[i][j + 1];
    }
  }

  // allocate new matrix
  graph->matrix = reallocMatrix(graph->matrix, oldDimension, oldDimension - 1);

  // update mappings
  graph->cityIdToIndex[sourceCityId] = -1;

  for(int i = removeIndex + 1; i < oldDimension; i++) {
    int cityId = graph->indexToCityId[i];

    graph->cityIdToIndex[cityId] = i - 1;
    graph->indexToCityId[i - 1] = cityId;
  }

  graph->currentDimension = newDimension;

  printf("City with id: %d was deleted\n", sourceCityId);
}

void editDistanceBetweenCitiesMatrix(MatrixGraphController* graph, int srcCityId, int destCityId, double newDistance) {
  int srcMatrixIdx = graph->cityIdToIndex[srcCityId];
  int destMatrixIdx = graph->cityIdToIndex[destCityId];
  if(srcMatrixIdx == -1 || destMatrixIdx == -1) {
    return;
  }

  graph->matrix[srcMatrixIdx][destMatrixIdx] = newDistance;
  graph->matrix[destMatrixIdx][srcMatrixIdx] = newDistance;
}

double getDistanceBetweenCitiesMatrix(MatrixGraphController* graph, int srcCityId, int destCityId) {
  int isOutOfBoundId = srcCityId < 0 || srcCityId > graph->maxCityId || destCityId < 0 || destCityId > graph->maxCityId;
  if(isOutOfBoundId) {
    return -1;
  }

  int srcMatrixIdx = graph->cityIdToIndex[srcCityId];
  int destMatrixIdx = graph->cityIdToIndex[destCityId];
  if(srcMatrixIdx == -1 || destMatrixIdx == -1) {
    return -1;
  }

  return graph->matrix[srcMatrixIdx][destMatrixIdx];
}

void freeEntireGraphMatrix(MatrixGraphController* graph) {
  for(int i = 0; i < graph->currentDimension; i++) {
    free(graph->matrix[i]);
  }
  free(graph->matrix);
  free(graph->cityIdToIndex);
  free(graph->indexToCityId);
  free(graph);
}

void matrixImplementation(City cities[], int totalCities) {
  MatrixGraphController* graph = malloc(sizeof(MatrixGraphController));
  graph->currentDimension = totalCities;
  graph->maxCityId = totalCities;

  double** matrix = malloc(sizeof(double*) * totalCities);
  for(int i = 0; i < totalCities; i++) {
    // starting all as 0's (that means no edge between these nodes)
    matrix[i] = calloc(totalCities, sizeof(double));
  }
  graph->matrix = matrix;

  // cityIdToIndex[1] = curr index on matrix of city with id 1
  graph->cityIdToIndex = malloc(sizeof(int) * (totalCities + 1));
  for(int i = 0; i <= totalCities; i++) {
    graph->cityIdToIndex[i] = -1;
  }
  graph->indexToCityId = malloc(sizeof(int) * (totalCities));

  // first do the mapping since we will need these mappings for the add edge func
  for(int i = 0; i < totalCities; i++) {
    int cityId = cities[i].id;
    graph->cityIdToIndex[cityId] = i;
    graph->indexToCityId[i] = cityId;
  }

  // complete graph -> every node has an edge to every other nodes
  for(int i = 0; i < totalCities; i++) {
    // j=i+1 because we need to skip diagonal and symetric, example:
    // avoid [0][0] and also avoid [1][2] and then [2][1]
    for(int j = i + 1; j < totalCities; j++) {
      double distance = calcCitiesEuclideanDistance(cities[i], cities[j]);
      addCitiesEdgeMatrix(graph, cities[i].id, cities[j].id, distance);
    }
  }

  printf("distance between cities 1 and 1: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 1));
  printf("distance between cities 1 and 2: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 2));
  printf("distance between cities 3 and 4: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 3, 4));
  printf("distance between cities 4 and 4: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 4, 4));

  removeCityAndAllEdgesMatrix(graph, 1);
  printf("distance between cities 1 and 2: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 2));
  printf("distance between cities 1 and 3: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 3));

  printf("distance between cities 3 and 10 (city 10 doesn't exist): %.2f\n",
         getDistanceBetweenCitiesMatrix(graph, 3, 10));

  freeEntireGraphMatrix(graph);
}

int main() {
  City cities[] = {{1, 24748.3333, 50840.0000},
                   {2, 24758.8889, 51211.9444},
                   {3, 25000.0000, 51000.0000},
                   {4, 24900.0000, 50900.0000}};
  int totalCities = sizeof(cities) / sizeof(cities[0]);

  // matrix
  matrixImplementation(cities, totalCities);

  // adjacency list

  return 0;
}