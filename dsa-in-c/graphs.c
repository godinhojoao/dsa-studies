/*
Deleting the executable after running:
gcc graphs.c -o test && ./test && rm -rf ./test
*/

/*
Undirected and complete graph for cities, version with adjacency matrix and also adjacency list:

Time and Space Complexity:

v = vertices = nodes = cities
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
- addNewCityWithAllEdgesListGraph:
  - time: O(v) loop v-1 cities to connect this city with others
  - space: O(v) because of the new v-1 new edges (undirected graph we connect this new city with all others)
- removeCityAndAllEdgesListGraph:
  - time: O(v+e) for each city we access its edges to remove connection with removed city
  - space: O(1) dont alloc any new space
- getDistanceBetweenCitiesListGraph:
  - time: O(V) if the graph is dense edges of v ~= V, so: O(V)
    - because we iterate all edges of v.
  - space: O(1) dont alloc any new space for this.
- editDistanceBetweenCitiesListGraph:
  - time: O(V) if the graph is dense edges of v ~= V, so: O(V)
    - because we iterate all edges of v.
  - space: O(1) dont alloc any new space for this.
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

// matrix start
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
  // printf("calcCitiesEuclideanDistance debug: source.id: %d to destination.id %d -> distance: %.2f\n",
  // source.id,destination.id, distance);
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
  printf("edited city %d to city %d newdistance = %.2f\n", srcCityId, destCityId, newDistance);
}

double getDistanceBetweenCitiesMatrix(MatrixGraphController* graph, int srcCityId, int destCityId) {
  if(srcCityId == destCityId) {
    return 0;
  }

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

  printf("\n--------------------testing matrix impl start--------------------\n");
  printf("distance between cities 1 and 1: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 1));
  printf("distance between cities 1 and 2: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 2));
  printf("distance between cities 3 and 4: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 3, 4));
  printf("distance between cities 4 and 4: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 4, 4));

  editDistanceBetweenCitiesMatrix(graph, 3, 2, 10);
  printf("distance between cities 3 and 2: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 3, 2));
  printf("distance between cities 2 and 3: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 2, 3));

  removeCityAndAllEdgesMatrix(graph, 1);
  printf("distance between cities 1 and 2: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 2));
  printf("distance between cities 1 and 3: %.2f\n", getDistanceBetweenCitiesMatrix(graph, 1, 3));

  printf("distance between cities 3 and 10 (city 10 doesn't exist): %.2f\n",
         getDistanceBetweenCitiesMatrix(graph, 3, 10));
  printf("--------------------testing matrix impl end --------------------\n\n");

  freeEntireGraphMatrix(graph);
}
// matrix end

// adjacency list start
struct Edge {
  int destId;
  double distance;
  struct Edge* next;
} typedef Edge;

struct AdjListGraph {
  int totalVertices;
  Edge** adjLists;
  // idea
  // adjListGraph->adjLists[5] represents the city with id 5
  // adjListGraph->adjLists[5] if null = city exist but is without connections, just a node
  // adjListGraph->adjLists[5] if edge information = city with connection to destId
} typedef AdjListGraph;

Edge* createEdge(int destId, double distance) {
  Edge* newEdge = malloc(sizeof(Edge));
  newEdge->destId = destId;
  newEdge->distance = distance;
  newEdge->next = NULL;
  return newEdge;
}

void addBothWaysConnectionBetweenCitiesListGraph(AdjListGraph* graph, int srcCityId, int destCityId, double distance) {
  int srcIdx = srcCityId - 1;
  int destIdx = destCityId - 1;
  if(srcIdx < 0 || srcIdx >= graph->totalVertices || destIdx < 0 || destIdx >= graph->totalVertices) {
    printf("addBothWaysConnectionBetweenCitiesListGraph: one invalid id was provided\n");
    return;
  }

  // inserting on head O(1)
  Edge* edgeFromSrcToDest = createEdge(destCityId, distance);
  edgeFromSrcToDest->next = graph->adjLists[srcIdx];
  graph->adjLists[srcIdx] = edgeFromSrcToDest;

  Edge* edgeFromDestToSrc = createEdge(srcCityId, distance);
  edgeFromDestToSrc->next = graph->adjLists[destIdx];
  graph->adjLists[destIdx] = edgeFromDestToSrc;
}

void addNewCityWithAllEdgesListGraph(AdjListGraph* graph, City cities[], int cityToInsertId) {
  int srcIdx = cityToInsertId - 1;
  if(srcIdx != graph->totalVertices) {
    // in my workaround I use cityId-1 as idx because I ENSURE input city id is always +1 incremented
    printf("addNewCityWithAllEdgesListGraph: an invalid id was provided\n");
    return;
  }

  Edge** newAdjListAfterRealloc = realloc(graph->adjLists, sizeof(Edge*) * (graph->totalVertices + 1));
  if(newAdjListAfterRealloc == NULL) {
    printf("realloc failed at addNewCityWithAllEdgesListGraph..\n");
    exit(1);
  }
  graph->adjLists = newAdjListAfterRealloc;
  graph->adjLists[srcIdx] = NULL;
  graph->totalVertices++;

  for(int i = 0; i < graph->totalVertices - 1; i++) {
    double distance = calcCitiesEuclideanDistance(cities[i], cities[cityToInsertId - 1]);
    addBothWaysConnectionBetweenCitiesListGraph(graph, i + 1, cityToInsertId, distance);
  }
}

void removeCityAndAllEdgesListGraph(AdjListGraph* graph, int cityToDeleteId) {
  int idxToRemove = cityToDeleteId - 1;
  if(idxToRemove < 0 || idxToRemove >= graph->totalVertices) {
    printf("removeCityAndAllEdgesListGraph invalid city id\n");
    return;
  }

  for(int i = 0; i < graph->totalVertices; i++) {
    if(i == idxToRemove) {
      continue;  // skip deleted one of copying
    }

    Edge* prevCityNodeTemp = NULL;
    Edge* currCityNodeTemp = graph->adjLists[i];

    // unlink the removed city inside the others and free edge between them
    while(currCityNodeTemp != NULL) {
      int isEdgeToRemove = currCityNodeTemp->destId == cityToDeleteId;
      if(!isEdgeToRemove) {
        prevCityNodeTemp = currCityNodeTemp;
        currCityNodeTemp = currCityNodeTemp->next;
        continue;
      }

      Edge* nextEdge = currCityNodeTemp->next;
      int isCityToRemoveAtFirstEdge = prevCityNodeTemp == NULL;
      if(isCityToRemoveAtFirstEdge) {
        graph->adjLists[i] = nextEdge;  // first, just skip the one to remove
      } else {
        prevCityNodeTemp->next =
            nextEdge;  // not first so we need to do that in order to keep other nodes in the linked list.
      }
      free(currCityNodeTemp);
      currCityNodeTemp = nextEdge;
    }
  }

  // free all edges of the city itself
  Edge* currEdge = graph->adjLists[idxToRemove];
  while(currEdge != NULL) {
    Edge* tempNextEdge = currEdge->next;
    free(currEdge);
    currEdge = tempNextEdge;
  }

  graph->adjLists[idxToRemove] = NULL;  // mark city as null = deleted

  printf("City with id: %d was deleted\n", cityToDeleteId);
}

void editDistanceBetweenCitiesListGraph(AdjListGraph* graph, int srcCityId, int destCityId, double newDistance) {
  int srcIdx = srcCityId - 1;
  int destIdx = destCityId - 1;
  if(srcIdx < 0 || srcIdx >= graph->totalVertices || destIdx < 0 || destIdx >= graph->totalVertices) {
    printf("editDistanceBetweenCitiesListGraph: one invalid id was provided\n");
    return;
  }

  if(graph->adjLists[srcIdx] == NULL || graph->adjLists[destIdx] == NULL) {
    printf("editDistanceBetweenCitiesListGraph one invalid id was provided\n");
    return;
  }

  Edge* srcToDestCurrEdge = graph->adjLists[srcIdx];
  while(srcToDestCurrEdge != NULL && srcToDestCurrEdge->destId != destCityId) {
    srcToDestCurrEdge = srcToDestCurrEdge->next;
  }

  if(srcToDestCurrEdge == NULL) {
    printf("editDistanceBetweenCitiesListGraph city %d isn't connected to city %d, invalid edit\n", srcCityId,
           destCityId);
    return;
  }
  srcToDestCurrEdge->distance = newDistance;

  // now the inverse since this is an undirected graph
  Edge* destToSrcCurrentEdge = graph->adjLists[destIdx];
  while(destToSrcCurrentEdge != NULL && destToSrcCurrentEdge->destId != srcCityId) {
    destToSrcCurrentEdge = destToSrcCurrentEdge->next;
  }
  destToSrcCurrentEdge->distance = newDistance;

  printf("edited city %d to city %d newdistance = %.2f\n", srcCityId, destCityId, newDistance);
}

double getDistanceBetweenCitiesListGraph(AdjListGraph* graph, int srcCityId, int destCityId) {
  if(srcCityId == destCityId) {
    return 0;
  }

  int srcIdx = srcCityId - 1;
  int destIdx = destCityId - 1;

  if(srcIdx < 0 || srcIdx >= graph->totalVertices || destIdx < 0 || destIdx >= graph->totalVertices) {
    return -1;
  }

  if(graph->adjLists[srcIdx] == NULL || graph->adjLists[destIdx] == NULL) {
    return -1;
  }

  Edge* currEdge = graph->adjLists[srcIdx];
  while(currEdge != NULL) {
    if(currEdge->destId == destCityId) {
      return currEdge->distance;
    }
    currEdge = currEdge->next;
  }
  return -1;  // disconnected cities
}

void freeEntireGraphList(AdjListGraph* graph) {  // O(V+E)
  if(graph == NULL) {
    return;
  }

  for(int i = 0; i < graph->totalVertices; i++) {  // vertices
    Edge* currEdge = graph->adjLists[i];
    while(currEdge != NULL) {  // edges of current node (not all edges) this is the reason is v+e instead of times e
      Edge* tempEdge = currEdge->next;
      free(currEdge);
      currEdge = tempEdge;
    }
  }
  free(graph->adjLists);
  free(graph);
}

void adjacencyListImplementation(City cities[], int totalCities) {
  AdjListGraph* graph = malloc(sizeof(AdjListGraph));
  graph->totalVertices = totalCities;
  graph->adjLists = malloc(sizeof(Edge*) * graph->totalVertices);

  for(int i = 0; i < totalCities; i++) {
    graph->adjLists[i] = NULL;  // initialize all city nodes as null
  }

  for(int i = 0; i < totalCities; i++) {
    for(int j = i + 1; j < totalCities; j++) {
      double distance = calcCitiesEuclideanDistance(cities[i], cities[j]);
      // complete graph -> every node has an edge to every other nodes
      addBothWaysConnectionBetweenCitiesListGraph(graph, cities[i].id, cities[j].id, distance);
    }
  }

  printf("\n--------------------testing adjacency list impl start--------------------\n");
  printf("distance between cities 1 and 1: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 1, 1));
  printf("distance between cities 1 and 2: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 1, 2));
  printf("distance between cities 3 and 4: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 3, 4));
  printf("distance between cities 4 and 4: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 4, 4));

  editDistanceBetweenCitiesListGraph(graph, 3, 2, 10);
  printf("distance between cities 3 and 2: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 3, 2));
  printf("distance between cities 2 and 3: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 2, 3));

  removeCityAndAllEdgesListGraph(graph, 1);
  printf("distance between cities 1 and 2: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 1, 2));
  printf("distance between cities 1 and 3: %.2f\n", getDistanceBetweenCitiesListGraph(graph, 1, 3));

  printf("distance between cities 3 and 10 (city 10 doesn't exist): %.2f\n",
         getDistanceBetweenCitiesListGraph(graph, 3, 10));
  printf("--------------------testing adjacency list impl end --------------------\n\n");

  freeEntireGraphList(graph);
}
// adjacency list end

int main() {
  City cities[] = {{1, 24748.3333, 50840.0000},
                   {2, 24758.8889, 51211.9444},
                   {3, 25000.0000, 51000.0000},
                   {4, 24900.0000, 50900.0000}};
  int totalCities = sizeof(cities) / sizeof(cities[0]);

  matrixImplementation(cities, totalCities);

  adjacencyListImplementation(cities, totalCities);

  printf("Want to see Big O of funcs in the end? yes 1, no 0\n");
  int a = 0;
  scanf("%d", &a);
  if(a == 1) {
    printf(
        "Undirected and complete graph for cities, version with adjacency matrix and also adjacency list:\n\
      \n\
      Time and Space Complexity:\n\
      \n\
      v = vertices = nodes = cities\n\
      e = edges = path between cities\n\
      \n\
      Matrix big O notation (worst case):\n\
      - addCitiesEdgeMatrix:\n\
        - time: O(v^2) we need to copy all the values to the new matrix\n\
        - space: O(v^2) we realloc an entire new matrix\n\
      - removeCityAndAllEdgesMatrix:\n\
        - time: O(v^2) we need to copy all the values to the new matrix\n\
        - space: O(v^2) we realloc an entire new matrix\n\
      - getDistanceBetweenCitiesMatrix:\n\
        - time: O(1) because we access by indexes directly\n\
        - space: O(1) dont alloc any new space for this\n\
      - editDistanceBetweenCitiesMatrix:\n\
        - time: O(1) because we access by indexes directly\n\
        - space: O(1) dont alloc any new space for this\n\
      \n\
      Adjacency list:\n\
      - addNewCityWithAllEdgesListGraph:\n\
        - time: O(v) loop v-1 cities to connect this city with others\n\
        - space: O(v) because of the new v-1 edges (undirected graph connects the new city with all others)\n\
      - removeCityAndAllEdgesListGraph:\n\
        - time: O(v + e) we traverse vertices and their edges to remove connections with the deleted city\n\
        - space: O(1) dont alloc any new space\n\
      - getDistanceBetweenCitiesListGraph:\n\
        - time: O(deg(v)) -> worst case O(v) in a dense graph, because we iterate all edges of the source vertex\n\
        - space: O(1) dont alloc any new space for this\n\
      - editDistanceBetweenCitiesListGraph:\n\
        - time: O(deg(v)) -> worst case O(v) in a dense graph, because we iterate edges of both vertices\n\
        - space: O(1) dont alloc any new space for this\n");
  }

  return 0;
}