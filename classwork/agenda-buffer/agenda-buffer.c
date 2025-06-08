/*
Deleting the executable after running:
gcc agenda-buffer.c -o test && ./test < input.txt && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_BUFFER_SIZE_IN_BYTES sizeof(int) * 2 + sizeof(char) * 30  // operation, usersAmount, searchName
#define USER_INFOS_SIZE_IN_BYTES (sizeof(int) + sizeof(char) * 30 * 2)  // age, name, email

void addUser(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  int* currUsersAmount = (int*)myBuffer + 1;  // skip age and get usersAmount

  myBuffer = realloc(myBuffer, START_BUFFER_SIZE_IN_BYTES + USER_INFOS_SIZE_IN_BYTES * (*currUsersAmount + 1));
  if(!myBuffer) {
    printf("error on realloc - add");
    printf("realloc failed");
    exit(1);
  }
  *bufferPtr = myBuffer;  // update original ptr with new buffer

  // after realloc, old pointer is freed and this new one (bigger) is created
  int* usersAmount = (int*)myBuffer + 1;
  int* agePtr = (int*)((char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + USER_INFOS_SIZE_IN_BYTES * *currUsersAmount);
  char* namePtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + USER_INFOS_SIZE_IN_BYTES * *currUsersAmount + sizeof(int);
  char* emailPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + USER_INFOS_SIZE_IN_BYTES * *currUsersAmount + sizeof(int) + 30 * sizeof(char);

  printf("User name: ");
  scanf("%s", namePtr);
  printf("User email: ");
  scanf("%s", emailPtr);
  printf("User age: ");
  scanf("%d", agePtr);

  (*usersAmount) = *currUsersAmount + 1;
}

void listUsers(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  int* usersAmount = (int*)myBuffer + 1;
  if(*usersAmount < 1) {
    printf("Empty users list.");
    return;
  }

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* lastUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + (*usersAmount * USER_INFOS_SIZE_IN_BYTES);

  while(currentUserPtr < lastUserPtr) {
    int* age = (int*)currentUserPtr;
    char* name = (char*)currentUserPtr + sizeof(int);
    char* email = (char*)currentUserPtr + sizeof(int) + sizeof(char) * 30;
    printf("\nname: %s - email: %s - age: %d", name, email, *age);
    currentUserPtr += USER_INFOS_SIZE_IN_BYTES;
  }
}

void searchByName(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  int* usersAmount = (int*)myBuffer + 1;
  if(*usersAmount < 1) {
    printf("Empty users list.");
    return;
  }

  char* searchName = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - 30;
  printf("User name to search: ");
  scanf("%s", searchName);

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* lastUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + (*usersAmount * USER_INFOS_SIZE_IN_BYTES);

  while(currentUserPtr < lastUserPtr) {
    char* name = (char*)currentUserPtr + sizeof(int);
    if(strcmp(name, searchName) == 0) {
      int* age = (int*)currentUserPtr;
      char* email = (char*)currentUserPtr + sizeof(int) + sizeof(char) * 30;
      printf("\nFound-> name: %s - email: %s - age: %d", name, email, *age);
      strcpy(searchName, "");
      return;
    }
    currentUserPtr += USER_INFOS_SIZE_IN_BYTES;
  }
  printf("User: %s not found!\n", searchName);
  strcpy(searchName, "");
}

void removeUserByName(void** bufferPtr) {
  void* myBuffer = *bufferPtr;
  int* usersAmount = (int*)myBuffer + 1;
  if(*usersAmount < 1) {
    printf("Empty users list.");
    return;
  }

  char* searchName = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES - 30;
  printf("User name to remove: ");
  scanf("%s", searchName);

  char* currentUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES;
  char* lastUserPtr = (char*)myBuffer + START_BUFFER_SIZE_IN_BYTES + (*usersAmount * USER_INFOS_SIZE_IN_BYTES);

  while(currentUserPtr < lastUserPtr) {
    char* name = (char*)currentUserPtr + sizeof(int);
    if(strcmp(name, searchName) == 0) {
      printf("\nDeleted user: %s", name);
      strcpy(searchName, "");

      // all in front of this user, go one position behind (one step back)
      char* frontUser = (char*)currentUserPtr + USER_INFOS_SIZE_IN_BYTES;
      while(frontUser < lastUserPtr) {
        int* frontUserAge = (int*)frontUser;
        char* frontUserName = (char*)frontUser + sizeof(int);
        char* frontUserEmail = (char*)frontUser + sizeof(int) + sizeof(char) * 30;

        int* newAgePosition = (int*)((char*)frontUserAge - ((sizeof(char) * 30 * 2) + sizeof(int)));  // return email, name and age
        char* newNamePosition = (char*)frontUserAge - sizeof(char) * 30 * 2;  // return email, name
        char* newEmailPosition = (char*)frontUserAge - sizeof(char) * 30;  // return email
        *newAgePosition = *frontUserAge;
        strcpy(newNamePosition, frontUserName);
        strcpy(newEmailPosition, frontUserEmail);

        frontUser += USER_INFOS_SIZE_IN_BYTES;
      }

      *usersAmount -= 1;
      myBuffer = realloc(myBuffer, START_BUFFER_SIZE_IN_BYTES + (USER_INFOS_SIZE_IN_BYTES * *usersAmount));
      if(!myBuffer) {
        printf("error on realloc - remove");
        exit(1);
      }
      *bufferPtr = myBuffer;  // update original ptr with new buffer
      return;
    }
    currentUserPtr += USER_INFOS_SIZE_IN_BYTES;
  }

  printf("User: %s not found!\n", searchName);
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
  *((int*)buffer + 1) = 0;  // starting users amount as 0

  menu(&buffer);

  free(buffer);
  return 0;
}