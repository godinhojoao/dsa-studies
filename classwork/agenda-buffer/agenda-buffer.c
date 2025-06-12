/*
Deleting the executable after running:
gcc agenda-buffer.c -o test && ./test < input.txt && rm -rf ./test
gcc agenda-buffer.c -o test && ./test && rm -rf ./test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_MAX_SIZE 300
// operation, currentUsersSize, searchName, tempName, tempEmail
#define START_BUFFER_SIZE_IN_BYTES sizeof(int) + sizeof(size_t) + sizeof(char) * STRING_MAX_SIZE * 3

void addUser(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  char* tempName = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE * 2;
  char* tempEmail = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - STRING_MAX_SIZE;

  printf("User name: ");
  scanf("%s", tempName);
  printf("User email: ");
  scanf("%s", tempEmail);

  // +1 on length to include \0 for string end
  size_t newUserSize = sizeof(int) + strlen(tempName) + 1 + strlen(tempEmail) + 1;
  size_t* currentUsersSize = (size_t*)((int*)myBuffer + 1);

  myBuffer = realloc(myBuffer, START_BUFFER_SIZE_IN_BYTES + *currentUsersSize + newUserSize);
  if(!myBuffer) {
    printf("error on realloc - add");
    printf("realloc failed");
    exit(1);
  }
  *bufferPtr = myBuffer;  // update original ptr with new buffer

  // after realloc, old pointer is freed and this new one (bigger) is created
  int* agePtr = (int*)((char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + *currentUsersSize);
  char* namePtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + *currentUsersSize + sizeof(int);
  char* emailPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + *currentUsersSize + sizeof(int) + (strlen(tempName) + 1);

  // saving data in the right places within the buffer
  strcpy(namePtr, tempName);
  strcpy(emailPtr, tempEmail);
  printf("User age: ");
  scanf("%d", agePtr);
  *currentUsersSize += newUserSize;

  strcpy(tempName, "");
  strcpy(tempEmail, "");
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

void searchByName(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  size_t* currentUsersSize = (size_t*)((int*)myBuffer + 1);

  if(*currentUsersSize == 0) {
    printf("Empty users list.");
    return;
  }

  char* searchName = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - 30;
  printf("User name to search: ");
  scanf("%s", searchName);

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* endOfUsersPtr = currentUserPtr + *currentUsersSize;

  while(currentUserPtr < endOfUsersPtr) {
    int* age = (int*)currentUserPtr;
    char* name = (char*)currentUserPtr + sizeof(int);
    char* email = (char*)currentUserPtr + sizeof(int) + strlen(name) + 1;

    if(strcmp(name, searchName) == 0) {
      printf("\nFound-> name: %s - email: %s - age: %d", name, email, *age);
      strcpy(searchName, "");
      return;
    }

    currentUserPtr += sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
  }
  printf("User: %s not found!", searchName);

  strcpy(searchName, "");
}

void removeUserByName(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  size_t* currentUsersSize = (size_t*)((int*)myBuffer + 1);

  if(*currentUsersSize == 0) {
    printf("Empty users list.");
    return;
  }

  char* searchName = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - 30;
  printf("User name to search: ");
  scanf("%s", searchName);

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* endOfUsersPtr = currentUserPtr + *currentUsersSize;

  while(currentUserPtr < endOfUsersPtr) {
    int* age = (int*)currentUserPtr;
    char* name = (char*)currentUserPtr + sizeof(int);
    char* email = (char*)currentUserPtr + sizeof(int) + strlen(name) + 1;

    if(strcmp(name, searchName) == 0) {
      printf("\nDeleted user: %s", name);
      strcpy(searchName, "");

      // all in front of this user, go one position behind (one step back)
      char* frontUser = (char*)currentUserPtr + sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
      while(frontUser < endOfUsersPtr) {
        int* frontUserAge = (int*)frontUser;
        char* frontUserName = (char*)frontUser + sizeof(int);
        char* frontUserEmail = (char*)frontUser + sizeof(int) + strlen(name) + 1;

        int* newAgePosition = (int*)((char*)frontUserAge - (strlen(name) + 1 + strlen(email) + 1 + sizeof(int)));  // return email, name and age
        char* newNamePosition = (char*)frontUserAge - strlen(name) + 1 + strlen(email) + 1;  // return email, name
        char* newEmailPosition = (char*)frontUserAge - strlen(email) + 1;  // return email

        *newAgePosition = *frontUserAge;
        strcpy(newNamePosition, frontUserName);
        strcpy(newEmailPosition, frontUserEmail);

        frontUser += sizeof(int) + strlen(frontUserName) + 1 + strlen(frontUserEmail) + 1;
      }

      *currentUsersSize -= strlen(name) + 1 + strlen(email) + 1 + sizeof(int);
      myBuffer = realloc(myBuffer, START_BUFFER_SIZE_IN_BYTES + *currentUsersSize);
      if(!myBuffer) {
        printf("error on realloc - remove");
        exit(1);
      }
      *bufferPtr = myBuffer;  // update original ptr with new buffer
      return;
    }

    currentUserPtr += sizeof(int) + strlen(name) + 1 + strlen(email) + 1;
  }

  printf("\nUser: %s not found!", searchName);
  strcpy(searchName, "");
}

void menu(void** bufferPtr) {
  int* currOperationPtr = (int*)*bufferPtr;

  while(*currOperationPtr != 5) {
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
        removeUserByName(bufferPtr);
        printf("\n---- Remove user ----\n");
        break;
      case 3:
        printf("\n---- Search user ----\n");
        searchByName(bufferPtr);
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

  menu(&buffer);

  free(buffer);
  return 0;
}