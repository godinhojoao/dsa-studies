/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* initNode() {
  struct ListNode* myNode = malloc(sizeof(struct ListNode));
  myNode->next = NULL;
  return myNode;
}

struct ListNode* appendNextNode(struct ListNode* tail, int value) {
  struct ListNode* newNode = initNode();
  newNode->val = value;
  tail->next = newNode;
  return newNode;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
  if(listsSize == 0) return NULL;
  if(listsSize == 1) return lists[0];

  struct ListNode* headToReturn = initNode();
  struct ListNode* tail = NULL;

  // init k pointers and lists
  struct ListNode* currentListsNodes[listsSize];  // node atual que eu to em cada lista
  int i = 0;
  while(i < listsSize) {
    currentListsNodes[i] = lists[i];
    i++;
  }
  i = 0;

  // agora eu preciso pegar os K valores inicias das listas pra comparar, se um for null ignoro ele:
  // ex: list[i]->val list[i+1]->val (mas dinamico, sem hard coded variables)
  while(i < listsSize) {  // k listas
    int currArrValues[listsSize];  // k ints pra guardar e comparar agora
    int smallest = -1;

    int j = 0;
    /*
        algo tipo:
        while(currentListsNodes[i] != NULL && currentListsNodes[i]->val) { // lista atual tem valor
            if(atual < smallest) {
                currentListsNodes[i] = currentListsNodes[i]->next;
            }
        }
    */
    // Arrumar aq ta o erro, ao inves de percorrer k vezes (estatico), percorrer enquanto um nao for nulo
    // visto que se node == NULL fim da lista
    while(j < listsSize) {
      if(currentListsNodes[j] != NULL && currentListsNodes[j]->val && (currentListsNodes[j]->val < smallest || smallest == -1)) {
        smallest = currentListsNodes[j]->val;
        if(tail == NULL) {
          headToReturn->val = smallest;
          tail = headToReturn;
        } else {
          struct ListNode* insertedNode = appendNextNode(tail, smallest);
          tail = insertedNode;
        }
      }
      j++;
    }
    i++;
  }

  return headToReturn;
}