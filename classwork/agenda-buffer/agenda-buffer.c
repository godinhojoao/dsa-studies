/*
Deleting the executable after running:
gcc agenda-buffer.c -o test && ./test < input.txt && rm -rf ./test
gcc agenda-buffer.c -o test && ./test && rm -rf ./test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_MAX_SIZE 300
// operation, currentUsersSize, searchEmail, tempName, tempEmail
#define START_BUFFER_SIZE_IN_BYTES sizeof(int) + sizeof(size_t) + sizeof(char) * STRING_MAX_SIZE * 3

int searchByEmail(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  size_t* currentUsersSize = (size_t*)((int*)myBuffer + 1);

  char* tempEmail = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE;
  if(*currentUsersSize == 0) {
    if(!tempEmail || tempEmail[0] == '\0') {  // if it exists we are on add user and don't want to log
      printf("Empty users list.");
    }
    return 0;
  }

  char* searchEmail = tempEmail;
  if(!tempEmail || tempEmail[0] == '\0') {
    searchEmail = (char*)myBuffer + sizeof(int) + sizeof(size_t);
    printf("User email to search: ");
    scanf("%s", searchEmail);
  }

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* endOfUsersPtr = currentUserPtr + *currentUsersSize;

  while(currentUserPtr < endOfUsersPtr) {
    int* age = (int*)currentUserPtr;
    char* name = (char*)currentUserPtr + sizeof(int);
    char* email = (char*)currentUserPtr + sizeof(int) + strlen(name) + 1;

    if(strcmp(email, searchEmail) == 0) {
      if(!tempEmail || tempEmail[0] == '\0') {  // if it exists we are on add user and don't want to log
        printf("\nFound-> name: %s - email: %s - age: %d", name, email, *age);
        strcpy(searchEmail, "");
      }
      return 1;
    }

    currentUserPtr += sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
  }

  if(!tempEmail || tempEmail[0] == '\0') {  // if it exists we are on add user and don't want to log
    printf("User with email %s not found!", searchEmail);
    strcpy(searchEmail, "");
  }
  return 0;
}

void addUser(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  char* tempName = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE * 2;
  char* tempEmail = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE;

  printf("User email: ");
  scanf("%s", tempEmail);
  if(searchByEmail(bufferPtr)) {
    printf("User with email %s already exists.\n", tempEmail);
    tempEmail[0] = '\0';
    return;
  }

  printf("User name: ");
  scanf("%s", tempName);

  // new buffer size = old size + new user size
  size_t* currentUsersSize = (size_t*)((char*)myBuffer + sizeof(int));
  myBuffer = realloc(myBuffer, START_BUFFER_SIZE_IN_BYTES + *currentUsersSize + sizeof(int) + (strlen(tempName) + 1) + (strlen(tempEmail) + 1));
  if(!myBuffer) {
    printf("error on realloc - add\n");
    exit(1);
  }
  *bufferPtr = myBuffer;

  // now we've reallocated our buffer. Old pointers were all freed, we need to "update" our pointers to valid references
  currentUsersSize = (size_t*)((char*)myBuffer + sizeof(int));
  tempName = (char*)myBuffer + sizeof(int) + sizeof(size_t) + STRING_MAX_SIZE;
  tempEmail = tempName + STRING_MAX_SIZE;

  int* agePtr = (int*)((char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + *currentUsersSize);
  char* namePtr = (char*)agePtr + sizeof(int);
  char* emailPtr = namePtr + strlen(tempName) + 1;

  strcpy(namePtr, tempName);
  strcpy(emailPtr, tempEmail);

  printf("User age: ");
  scanf("%d", agePtr);

  // update adding new user size on calc
  *currentUsersSize += sizeof(int) + (strlen(tempName) + 1) + (strlen(tempEmail) + 1);

  tempName[0] = '\0';
  tempEmail[0] = '\0';
}

void listUsers(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  size_t* currentUsersSize = (size_t*)((int*)myBuffer + 1);

  if(*currentUsersSize == 0) {
    printf("Empty users list.");
    return;
  }

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* endOfUsersPtr = currentUserPtr + *currentUsersSize;

  while(currentUserPtr < endOfUsersPtr) {
    int* age = (int*)currentUserPtr;
    char* name = (char*)currentUserPtr + sizeof(int);
    char* email = (char*)currentUserPtr + sizeof(int) + strlen(name) + 1;

    printf("\nname: %s - email: %s - age: %d", name, email, *age);

    currentUserPtr += sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
  }
}

void removeUserByEmail(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  size_t* currentUsersSize = (size_t*)((int*)myBuffer + 1);

  if(*currentUsersSize == 0) {
    printf("Empty users list.");
    return;
  }

  char* searchEmail = (char*)myBuffer + sizeof(int) + sizeof(size_t);
  printf("User email to search: ");
  scanf("%s", searchEmail);

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* endOfUsersPtr = currentUserPtr + *currentUsersSize;

  while(currentUserPtr < endOfUsersPtr) {
    int* age = (int*)currentUserPtr;
    char* name = (char*)currentUserPtr + sizeof(int);
    char* email = name + strlen(name) + 1;

    if(strcmp(email, searchEmail) == 0) {
      printf("\nDeleted user with email: %s and name: %s \n",email, name);
      strcpy(searchEmail, "");

      // move front users one step back since we will remove one
      char* frontUser = currentUserPtr + sizeof(int) + strlen(name) + 1 + strlen(email) + 1;  // skip current user to get next
      while(frontUser < endOfUsersPtr) {
        int* frontUserAge = (int*)frontUser;
        char* frontUserName = (char*)frontUser + sizeof(int);
        char* frontUserEmail = frontUserName + strlen(frontUserName) + 1;

        int* newAgePosition = (int*)currentUserPtr;
        char* newNamePosition = (char*)newAgePosition + sizeof(int);
        char* newEmailPosition = newNamePosition + strlen(frontUserName) + 1;

        *newAgePosition = *frontUserAge;
        strcpy(newNamePosition, frontUserName);
        strcpy(newEmailPosition, frontUserEmail);

        // skip current "frontUser" to get next and so on..
        currentUserPtr += sizeof(int) + strlen(frontUserName) + 1 + strlen(frontUserEmail) + 1; // "prev"
        frontUser += sizeof(int) + strlen(frontUserName) + 1 + strlen(frontUserEmail) + 1; // "next"
      }

      // subtract current user size to realloc removing this user
      *currentUsersSize -= sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
      myBuffer = realloc(myBuffer, START_BUFFER_SIZE_IN_BYTES + *currentUsersSize);
      if(!myBuffer) {
        printf("error on realloc - remove");
        exit(1);
      }
      *bufferPtr = myBuffer;
      return;
    }

    // sum current user size to jump one user ahead
    currentUserPtr += sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
  }

  printf("\nUser with email %s not found!\n", searchEmail);
  strcpy(searchEmail, "");
}

void menu(void** bufferPtr) {
  while(1) {
    int* currOperationPtr = (int*)*bufferPtr;
    if(*currOperationPtr == 5) break;

    printf("\n--- MENU: Select one operation ---\n");
    printf("1 - Add user\n");
    printf("2 - Remove user by name\n");
    printf("3 - Search user by name\n");
    printf("4 - List all\n");
    printf("5 - Exit\n");
    printf("Current operation: ");
    scanf("%d", currOperationPtr);

    printf("\n");

    switch(*currOperationPtr) {
      case 1:
        printf("\n---- Add user ----\n");
        addUser(bufferPtr);
        printf("\n---- Add user ----\n");
        break;
      case 2:
        printf("\n---- Remove user ----\n");
        removeUserByEmail(bufferPtr);
        printf("\n---- Remove user ----\n");
        break;
      case 3:
        printf("\n---- Search user ----\n");
        searchByEmail(bufferPtr);
        printf("\n---- Search user ----\n");
        break;
      case 4:
        printf("\n---- List of all users ----\n");
        listUsers(bufferPtr);
        printf("\n---- List of all users ----\n");
        break;
      case 5:
        printf("exiting\n");
        break;
      default:
        printf("Invalid option, try again\n");
        *currOperationPtr = 0;
        break;
    }
  }
}

int main() {
  void* buffer = malloc(START_BUFFER_SIZE_IN_BYTES);
  *((int*)buffer) = 0;  // starting operation as 0
  *((size_t*)((int*)buffer + 1)) = 0;  // starting currentUsersSize as 0
  char* searchEmail = (char*)buffer + sizeof(int) + sizeof(size_t);
  char* tempName = (char*)buffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE * 2;
  char* tempEmail = (char*)buffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE;
  searchEmail[0] = '\0';
  tempName[0] = '\0';
  tempEmail[0] = '\0';

  menu(&buffer);

  free(buffer);
  return 0;
}