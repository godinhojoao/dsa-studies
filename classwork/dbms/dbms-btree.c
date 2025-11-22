/*
Deleting the executable after running:
gcc dbms-btree.c -o test && ./test && rm -rf ./test
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMUM_DEGREE_OF_BTREE 3  // t
#define MAX_KEYS 5  // 2t-1 = 5
#define MIN_KEYS 2  // t-1 = 2
#define MAX_CHILDREN 6  // 2t = 6
// min children = page's used keys + 1

typedef struct Page {
  int keys[MAX_KEYS];
  long offsets[MAX_KEYS];
  struct Page* children[MAX_CHILDREN];
  long childrenOffsets[MAX_CHILDREN];
  int amountOfKeys;
  int isLeaf;
} Page;

typedef struct Student {
  int student_id;  // pk
  char name[30];
  int deleted;
} Student;

typedef struct Subject {
  int subject_id;  // pk
  char name[30];
  int deleted;
} Subject;

typedef struct Enrollment {
  int enrollment_id;  // pk
  int student_id;  // fk student
  int subject_id;  // fk subject
  float final_grade;
  int deleted;
} Enrollment;

// ------ start btree code
void savePage(FILE* fp, Page* page, long* offset) {
  fseek(fp, 0, SEEK_END);
  *offset = ftell(fp) / sizeof(Page);  // current file size in bytes / page size = page index
  fwrite(page, sizeof(Page), 1, fp);
}

Page* loadPage(FILE* fp, long offset) {
  fseek(fp, offset * sizeof(Page), SEEK_SET);
  Page* page = malloc(sizeof(Page));
  fread(page, sizeof(Page), 1, fp);
  if(!page->isLeaf) {
    // recursively load all children from disk using their offsets
    for(int i = 0; i <= page->amountOfKeys; i++) {
      if(page->childrenOffsets[i] != -1) {
        page->children[i] = loadPage(fp, page->childrenOffsets[i]);
      } else {
        page->children[i] = NULL;
      }
    }
  } else {
    // leaf nodes have no children
    for(int i = 0; i < MAX_CHILDREN; i++) {
      page->children[i] = NULL;
    }
  }
  return page;
}

// writes page and all children recursively
void writePageAndChildren(FILE* fp, Page* node, long* offset) {
  if(!node) return;
  for(int i = 0; i <= node->amountOfKeys; i++) {
    if(node->children[i]) {
      writePageAndChildren(fp, node->children[i], &node->childrenOffsets[i]);
    }
  }
  savePage(fp, node, offset);
}

Page* createBTree(int rootKey, long rootOffset) {
  Page* rootPage = malloc(sizeof(Page));
  if(!rootPage) return NULL;

  rootPage->amountOfKeys = 1;
  rootPage->keys[0] = rootKey;
  rootPage->offsets[0] = rootOffset;
  for(int i = 0; i < MAX_CHILDREN; i++) {
    rootPage->children[i] = NULL;
    rootPage->childrenOffsets[i] = -1;
  }
  for(int i = 1; i < MAX_KEYS; i++) {
    rootPage->offsets[i] = -1;
  }
  rootPage->isLeaf = 1;
  return rootPage;
}

Page* btreeSearchKey(Page* currPage, int key) {
  if(!currPage) {
    return NULL;
  }

  int i = 0;
  // we go one further because we have n-1 keys and n children (if our key is bigger than all page keys, check last child)
  while(i < currPage->amountOfKeys && key > currPage->keys[i]) {
    i++;
  }

  // found
  if(i < currPage->amountOfKeys && key == currPage->keys[i]) {
    return currPage;
  }

  if(currPage->isLeaf) {
    // can't check children, because its leaf
    return NULL;
  }
  return btreeSearchKey(currPage->children[i], key);
}

// gets disk offset for key in btree
long btreeGetOffset(Page* currPage, int key) {
  if(!currPage) return -1;

  int i = 0;
  while(i < currPage->amountOfKeys && key > currPage->keys[i]) i++;

  if(i < currPage->amountOfKeys && key == currPage->keys[i]) return currPage->offsets[i];

  if(currPage->isLeaf) return -1;
  return btreeGetOffset(currPage->children[i], key);
}

// used on insert to split one node into two and promoting median key to the parent node
void splitChild(Page* parent, int childToSplitIndex) {
  int t = MINIMUM_DEGREE_OF_BTREE;
  Page* full = parent->children[childToSplitIndex];
  Page* right = malloc(sizeof(Page));
  right->isLeaf = full->isLeaf;
  right->amountOfKeys = t - 1;  // because we've splitted a full node 2*(t-1)/2 = t-1 (this is our median key)

  for(int j = 0; j < t - 1; j++) {
    right->keys[j] = full->keys[j + t];  // copy last t-1 keys from full node to new right node
    right->offsets[j] = full->offsets[j + t];
  }

  // if not leaf copy children to right node
  if(!full->isLeaf) {
    for(int j = 0; j < t; j++) {
      right->children[j] = full->children[j + t];
      right->childrenOffsets[j] = full->childrenOffsets[j + t];
    }
  }

  full->amountOfKeys = t - 1;

  // move keys bigger than childToSplitIndex one position ahead to open a space for the new promoted key
  for(int j = parent->amountOfKeys; j > childToSplitIndex; j--) {
    parent->keys[j] = parent->keys[j - 1];
    parent->offsets[j] = parent->offsets[j - 1];
  }
  parent->keys[childToSplitIndex] = full->keys[t - 1];  // median key
  parent->offsets[childToSplitIndex] = full->offsets[t - 1];
  for(int j = parent->amountOfKeys + 1; j > childToSplitIndex + 1; j--) {
    parent->children[j] = parent->children[j - 1];  // aligning children with the keys
    parent->childrenOffsets[j] = parent->childrenOffsets[j - 1];
  }
  parent->children[childToSplitIndex + 1] = right;  // left is children[i] and right is children[i+1]
  parent->childrenOffsets[childToSplitIndex + 1] = -1;
  parent->amountOfKeys++;
}

void insertNonFull(Page* node, int key, long offset) {
  int t = MINIMUM_DEGREE_OF_BTREE;
  int i = node->amountOfKeys - 1;

  if(node->isLeaf) {
    // here we will finally insert
    // push all keys larger than key to insert one position forward to open space for the new one
    while(i >= 0 && key < node->keys[i]) {
      node->keys[i + 1] = node->keys[i];
      node->offsets[i + 1] = node->offsets[i];
      i--;
    }
    node->keys[i + 1] = key;
    node->offsets[i + 1] = offset;
    node->amountOfKeys++;
  } else {
    // not leaf, find path to follow and insert on leaf
    while(i >= 0 && key < node->keys[i]) {
      i--;
    }
    // if the key to insert is bigger than key[i] we want to insert after position i and "i" can also be -1 here, because of that we do i++
    i++;
    // if the node we want to access is a full node split it before proceeding
    if(node->children[i]->amountOfKeys == 2 * t - 1) {
      splitChild(node, i);
      if(key > node->keys[i]) {
        i++;
      }
    }
    insertNonFull(node->children[i], key, offset);
  }
}

Page* insertBTree(Page* root, int key, long offset) {
  int t = MINIMUM_DEGREE_OF_BTREE;
  // root is full, create new root and split full node promoting median key to new root
  if(root->amountOfKeys == 2 * t - 1) {
    Page* newRoot = malloc(sizeof(Page));
    newRoot->isLeaf = 0;
    newRoot->amountOfKeys = 0;
    newRoot->children[0] = root;
    for(int i = 0; i < MAX_CHILDREN; i++) {
      newRoot->children[i] = newRoot->children[i] ? newRoot->children[i] : NULL;
      newRoot->childrenOffsets[i] = -1;
    }
    for(int i = 0; i < MAX_KEYS; i++) {
      newRoot->offsets[i] = -1;
    }
    splitChild(newRoot, 0);
    insertNonFull(newRoot, key, offset);
    return newRoot;
  } else {
    insertNonFull(root, key, offset);
    return root;
  }
}

int getPredecessor(Page* node, int i) {
  if(!node->children[i]) return -1;
  Page* curr = node->children[i];
  while(!curr->isLeaf && curr->amountOfKeys > 0 && curr->children[curr->amountOfKeys]) {
    curr = curr->children[curr->amountOfKeys];
  }
  if(curr && curr->amountOfKeys > 0) {
    return curr->keys[curr->amountOfKeys - 1];
  }
  return -1;
}

int getSuccessor(Page* node, int i) {
  if(!node->children[i + 1]) return -1;
  Page* curr = node->children[i + 1];
  while(!curr->isLeaf && curr->amountOfKeys > 0 && curr->children[0]) {
    curr = curr->children[0];
  }
  if(curr && curr->amountOfKeys > 0) {
    return curr->keys[0];
  }
  return -1;
}

void borrowFromLeft(Page* parent, int childIndex) {
  if(!parent || childIndex <= 0 || childIndex > parent->amountOfKeys) return;

  Page* child = parent->children[childIndex];
  Page* left = parent->children[childIndex - 1];

  if(!child || !left || left->amountOfKeys == 0) return;

  for(int i = child->amountOfKeys - 1; i >= 0; i--) {
    child->keys[i + 1] = child->keys[i];
    child->offsets[i + 1] = child->offsets[i];
  }
  if(!child->isLeaf) {
    for(int i = child->amountOfKeys; i >= 0; i--) {
      child->children[i + 1] = child->children[i];
      child->childrenOffsets[i + 1] = child->childrenOffsets[i];
    }
  }

  child->keys[0] = parent->keys[childIndex - 1];
  child->offsets[0] = parent->offsets[childIndex - 1];
  if(!child->isLeaf) {
    child->children[0] = left->children[left->amountOfKeys];
    child->childrenOffsets[0] = left->childrenOffsets[left->amountOfKeys];
  }

  parent->keys[childIndex - 1] = left->keys[left->amountOfKeys - 1];
  parent->offsets[childIndex - 1] = left->offsets[left->amountOfKeys - 1];

  child->amountOfKeys++;
  left->amountOfKeys--;
}

void borrowFromRight(Page* parent, int childIndex) {
  if(!parent || childIndex < 0 || childIndex >= parent->amountOfKeys) return;

  Page* child = parent->children[childIndex];
  Page* right = parent->children[childIndex + 1];

  if(!child || !right || right->amountOfKeys == 0) return;

  child->keys[child->amountOfKeys] = parent->keys[childIndex];
  child->offsets[child->amountOfKeys] = parent->offsets[childIndex];
  if(!child->isLeaf) {
    child->children[child->amountOfKeys + 1] = right->children[0];
    child->childrenOffsets[child->amountOfKeys + 1] = right->childrenOffsets[0];
  }

  parent->keys[childIndex] = right->keys[0];
  parent->offsets[childIndex] = right->offsets[0];

  for(int i = 1; i < right->amountOfKeys; i++) {
    right->keys[i - 1] = right->keys[i];
    right->offsets[i - 1] = right->offsets[i];
  }
  if(!right->isLeaf) {
    for(int i = 1; i <= right->amountOfKeys; i++) {
      right->children[i - 1] = right->children[i];
      right->childrenOffsets[i - 1] = right->childrenOffsets[i];
    }
  }

  child->amountOfKeys++;
  right->amountOfKeys--;
}

void mergeWithNext(Page* parent, int childIndex) {
  if(!parent || childIndex < 0 || childIndex >= parent->amountOfKeys) return;

  Page* child = parent->children[childIndex];
  Page* right = parent->children[childIndex + 1];
  if(!child || !right) return;

  int t = MINIMUM_DEGREE_OF_BTREE;

  child->keys[t - 1] = parent->keys[childIndex];
  child->offsets[t - 1] = parent->offsets[childIndex];
  for(int i = 0; i < right->amountOfKeys; i++) {
    child->keys[i + t] = right->keys[i];
    child->offsets[i + t] = right->offsets[i];
  }
  if(!child->isLeaf) {
    for(int i = 0; i <= right->amountOfKeys; i++) {
      child->children[i + t] = right->children[i];
      child->childrenOffsets[i + t] = right->childrenOffsets[i];
    }
  }

  child->amountOfKeys += right->amountOfKeys + 1;

  for(int i = childIndex + 1; i < parent->amountOfKeys; i++) {
    parent->keys[i - 1] = parent->keys[i];
    parent->offsets[i - 1] = parent->offsets[i];
  }
  for(int i = childIndex + 2; i <= parent->amountOfKeys; i++) {
    parent->children[i - 1] = parent->children[i];
    parent->childrenOffsets[i - 1] = parent->childrenOffsets[i];
  }

  parent->amountOfKeys--;
  free(right);
}

void removeFromNode(Page* node, int key) {
  if(!node) return;
  int keyIndex = 0;
  while(keyIndex < node->amountOfKeys && key > node->keys[keyIndex]) keyIndex++;

  if(keyIndex < node->amountOfKeys && key == node->keys[keyIndex]) {
    if(node->isLeaf) {
      for(int i = keyIndex + 1; i < node->amountOfKeys; i++) {
        node->keys[i - 1] = node->keys[i];
        node->offsets[i - 1] = node->offsets[i];
      }
      node->amountOfKeys--;
    } else {
      Page* predChild = node->children[keyIndex];
      Page* succChild = node->children[keyIndex + 1];
      if(predChild && predChild->amountOfKeys >= MIN_KEYS + 1) {
        int pred = getPredecessor(node, keyIndex);
        if(pred != -1) {
          node->keys[keyIndex] = pred;
          removeFromNode(predChild, pred);
        }
      } else if(succChild && succChild->amountOfKeys >= MIN_KEYS + 1) {
        int succ = getSuccessor(node, keyIndex);
        if(succ != -1) {
          node->keys[keyIndex] = succ;
          removeFromNode(succChild, succ);
        }
      } else {
        mergeWithNext(node, keyIndex);
        removeFromNode(predChild, key);
      }
    }
  } else {
    if(node->isLeaf) return;
    int flag = (keyIndex == node->amountOfKeys) ? 1 : 0;
    Page* child = node->children[keyIndex];
    if(!child) return;
    if(child->amountOfKeys == MIN_KEYS) {
      if(keyIndex != 0 && node->children[keyIndex - 1] && node->children[keyIndex - 1]->amountOfKeys > MIN_KEYS) {
        borrowFromLeft(node, keyIndex);
      } else if(keyIndex != node->amountOfKeys && node->children[keyIndex + 1] && node->children[keyIndex + 1]->amountOfKeys > MIN_KEYS) {
        borrowFromRight(node, keyIndex);
      } else {
        if(keyIndex != node->amountOfKeys) {
          mergeWithNext(node, keyIndex);
        } else {
          mergeWithNext(node, keyIndex - 1);
          child = node->children[keyIndex - 1];
        }
      }
    }
    if(child) removeFromNode(child, key);
  }
}

Page* removeBTree(Page* root, int key) {
  if(!root) return NULL;

  removeFromNode(root, key);

  if(root->amountOfKeys == 0) {
    Page* newRoot = root->isLeaf ? NULL : root->children[0];
    free(root);
    return newRoot;
  }

  return root;
}

// ------ end btree code

// ------ start index management

Page* studentsIndex = NULL;
Page* subjectsIndex = NULL;
Page* enrollmentsIndex = NULL;

Page* loadBTreeFromDisk(const char* idxFile) {
  FILE* fp = fopen(idxFile, "rb");
  if(!fp) return NULL;

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  if(size == 0) {
    fclose(fp);
    return NULL;
  }

  Page* root = loadPage(fp, 0);
  fclose(fp);
  return root;
}

void saveBTreeToDisk(Page* root, const char* idxFile) {
  if(!root) return;

  FILE* fp = fopen(idxFile, "wb");
  if(!fp) return;

  long offset = 0;
  writePageAndChildren(fp, root, &offset);
  fclose(fp);
}

void initializeIndexes() {
  studentsIndex = loadBTreeFromDisk("students.idx");
  if(!studentsIndex) {
    FILE* fp = fopen("students.dat", "rb");
    if(fp) {
      fseek(fp, 0, SEEK_END);
      long total = ftell(fp) / sizeof(Student);
      if(total > 0) {
        Student student;
        for(long i = 0; i < total; i++) {
          fseek(fp, i * sizeof(Student), SEEK_SET);
          fread(&student, sizeof(Student), 1, fp);
          if(!student.deleted) {
            if(!studentsIndex) {
              studentsIndex = createBTree(student.student_id, i);
            } else {
              studentsIndex = insertBTree(studentsIndex, student.student_id, i);
            }
          }
        }
        if(studentsIndex) {
          saveBTreeToDisk(studentsIndex, "students.idx");
        }
      }
      fclose(fp);
    }
  }

  subjectsIndex = loadBTreeFromDisk("subjects.idx");
  if(!subjectsIndex) {
    FILE* fp = fopen("subjects.dat", "rb");
    if(fp) {
      fseek(fp, 0, SEEK_END);
      long total = ftell(fp) / sizeof(Subject);
      if(total > 0) {
        Subject subject;
        for(long i = 0; i < total; i++) {
          fseek(fp, i * sizeof(Subject), SEEK_SET);
          fread(&subject, sizeof(Subject), 1, fp);
          if(!subject.deleted) {
            if(!subjectsIndex) {
              subjectsIndex = createBTree(subject.subject_id, i);
            } else {
              subjectsIndex = insertBTree(subjectsIndex, subject.subject_id, i);
            }
          }
        }
        if(subjectsIndex) {
          saveBTreeToDisk(subjectsIndex, "subjects.idx");
        }
      }
      fclose(fp);
    }
  }

  enrollmentsIndex = loadBTreeFromDisk("enrollments.idx");
  if(!enrollmentsIndex) {
    FILE* fp = fopen("enrollments.dat", "rb");
    if(fp) {
      fseek(fp, 0, SEEK_END);
      long total = ftell(fp) / sizeof(Enrollment);
      if(total > 0) {
        Enrollment enrollment;
        for(long i = 0; i < total; i++) {
          fseek(fp, i * sizeof(Enrollment), SEEK_SET);
          fread(&enrollment, sizeof(Enrollment), 1, fp);
          if(!enrollment.deleted) {
            if(!enrollmentsIndex) {
              enrollmentsIndex = createBTree(enrollment.enrollment_id, i);
            } else {
              enrollmentsIndex = insertBTree(enrollmentsIndex, enrollment.enrollment_id, i);
            }
          }
        }
        if(enrollmentsIndex) {
          saveBTreeToDisk(enrollmentsIndex, "enrollments.idx");
        }
      }
      fclose(fp);
    }
  }
}

// ------ start data file code

int pkExists(const char* filename, int key, size_t structSize, size_t pkOffset) {
  Page* index = NULL;
  if(strcmp(filename, "students.dat") == 0) {
    index = studentsIndex;
  } else if(strcmp(filename, "subjects.dat") == 0) {
    index = subjectsIndex;
  } else if(strcmp(filename, "enrollments.dat") == 0) {
    index = enrollmentsIndex;
  }

  if(index) {
    return btreeSearchKey(index, key) != NULL;
  }

  return 0;
}

void addStudent(int student_id, const char* name) {
  if(pkExists("students.dat", student_id, sizeof(Student), 0)) {
    printf("Error: Student ID %d already exists.\n", student_id);
    return;
  }

  FILE* fp = fopen("students.dat", "ab");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long offset = ftell(fp) / sizeof(Student);

  Student student = {student_id};
  strncpy(student.name, name, sizeof(student.name) - 1);
  student.name[sizeof(student.name) - 1] = 0;
  student.deleted = 0;
  fwrite(&student, sizeof(Student), 1, fp);
  fclose(fp);

  if(!studentsIndex) {
    studentsIndex = createBTree(student_id, offset);
  } else {
    studentsIndex = insertBTree(studentsIndex, student_id, offset);
  }
  saveBTreeToDisk(studentsIndex, "students.idx");
}

void addSubject(int subject_id, const char* name) {
  if(pkExists("subjects.dat", subject_id, sizeof(Subject), 0)) {
    printf("Error: Subject ID %d already exists.\n", subject_id);
    return;
  }

  FILE* fp = fopen("subjects.dat", "ab");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long offset = ftell(fp) / sizeof(Subject);

  Subject subject = {subject_id};
  strncpy(subject.name, name, sizeof(subject.name) - 1);
  subject.name[sizeof(subject.name) - 1] = 0;
  subject.deleted = 0;
  fwrite(&subject, sizeof(Subject), 1, fp);
  fclose(fp);

  if(!subjectsIndex) {
    subjectsIndex = createBTree(subject_id, offset);
  } else {
    subjectsIndex = insertBTree(subjectsIndex, subject_id, offset);
  }
  saveBTreeToDisk(subjectsIndex, "subjects.idx");
}

void addEnrollment(int enrollment_id, int student_id, int subject_id, float final_grade) {
  if(pkExists("enrollments.dat", enrollment_id, sizeof(Enrollment), 0)) {
    printf("Error: Enrollment ID %d already exists.\n", enrollment_id);
    return;
  }

  if(!pkExists("students.dat", student_id, sizeof(Student), 0)) {
    printf("Error: Student ID %d does not exist.\n", student_id);
    return;
  }

  if(!pkExists("subjects.dat", subject_id, sizeof(Subject), 0)) {
    printf("Error: Subject ID %d does not exist.\n", subject_id);
    return;
  }

  FILE* fp = fopen("enrollments.dat", "rb+");
  Enrollment enrollment;
  long total = 0;
  if(fp) {
    fseek(fp, 0, SEEK_END);
    total = ftell(fp) / sizeof(Enrollment);
    for(long i = 0; i < total; i++) {
      fseek(fp, i * sizeof(Enrollment), SEEK_SET);
      fread(&enrollment, sizeof(Enrollment), 1, fp);
      if(!enrollment.deleted && enrollment.student_id == student_id && enrollment.subject_id == subject_id) {
        printf("Error: Enrollment for student %d in subject %d already exists.\n", student_id, subject_id);
        fclose(fp);
        return;
      }
    }
    fclose(fp);
  }

  fp = fopen("enrollments.dat", "ab");
  fseek(fp, 0, SEEK_END);
  long offset = ftell(fp) / sizeof(Enrollment);

  enrollment.enrollment_id = enrollment_id;
  enrollment.student_id = student_id;
  enrollment.subject_id = subject_id;
  enrollment.final_grade = final_grade;
  enrollment.deleted = 0;
  fwrite(&enrollment, sizeof(Enrollment), 1, fp);
  fclose(fp);

  if(!enrollmentsIndex) {
    enrollmentsIndex = createBTree(enrollment_id, offset);
  } else {
    enrollmentsIndex = insertBTree(enrollmentsIndex, enrollment_id, offset);
  }
  saveBTreeToDisk(enrollmentsIndex, "enrollments.idx");
}

void updateEnrollmentGrade(int enrollment_id, float new_grade) {
  long offset = btreeGetOffset(enrollmentsIndex, enrollment_id);
  if(offset == -1) {
    printf("Enrollment ID %d not found.\n", enrollment_id);
    return;
  }

  FILE* fp = fopen("enrollments.dat", "rb+");
  if(!fp) {
    printf("No enrollments found.\n");
    return;
  }

  Enrollment enrollment;
  fseek(fp, offset * sizeof(Enrollment), SEEK_SET);
  fread(&enrollment, sizeof(Enrollment), 1, fp);

  if(enrollment.deleted) {
    printf("Enrollment ID %d not found.\n", enrollment_id);
    fclose(fp);
    return;
  }

  enrollment.final_grade = new_grade;
  fseek(fp, offset * sizeof(Enrollment), SEEK_SET);
  fwrite(&enrollment, sizeof(Enrollment), 1, fp);
  fclose(fp);

  printf("Enrollment ID %d updated with new grade %.2f.\n", enrollment_id, new_grade);
}

void removeEnrollmentsByStudent(int student_id) {
  FILE* fp = fopen("enrollments.dat", "rb+");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long total = ftell(fp) / sizeof(Enrollment);
  Enrollment enrollment;
  for(long i = 0; i < total; i++) {
    fseek(fp, i * sizeof(Enrollment), SEEK_SET);
    fread(&enrollment, sizeof(Enrollment), 1, fp);
    if(enrollment.student_id == student_id && !enrollment.deleted) {
      enrollment.deleted = 1;
      fseek(fp, i * sizeof(Enrollment), SEEK_SET);
      fwrite(&enrollment, sizeof(Enrollment), 1, fp);
      enrollmentsIndex = removeBTree(enrollmentsIndex, enrollment.enrollment_id);
    }
  }

  fclose(fp);
  saveBTreeToDisk(enrollmentsIndex, "enrollments.idx");
}

void removeEnrollmentsBySubject(int subject_id) {
  FILE* fp = fopen("enrollments.dat", "rb+");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long total = ftell(fp) / sizeof(Enrollment);
  Enrollment enrollment;
  for(long i = 0; i < total; i++) {
    fseek(fp, i * sizeof(Enrollment), SEEK_SET);
    fread(&enrollment, sizeof(Enrollment), 1, fp);
    if(enrollment.subject_id == subject_id && !enrollment.deleted) {
      enrollment.deleted = 1;
      fseek(fp, i * sizeof(Enrollment), SEEK_SET);
      fwrite(&enrollment, sizeof(Enrollment), 1, fp);
      enrollmentsIndex = removeBTree(enrollmentsIndex, enrollment.enrollment_id);
    }
  }

  fclose(fp);
  saveBTreeToDisk(enrollmentsIndex, "enrollments.idx");
}

// Read (Get) functions
void getStudents() {
  FILE* fp = fopen("students.dat", "rb");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long total = ftell(fp) / sizeof(Student);

  Student student;
  for(long i = 0; i < total; i++) {
    fseek(fp, i * sizeof(Student), SEEK_SET);
    fread(&student, sizeof(Student), 1, fp);
    if(!student.deleted) {
      printf("ID: %d, Name: %s\n", student.student_id, student.name);
    }
  }

  fclose(fp);
}

void getSubjects() {
  FILE* fp = fopen("subjects.dat", "rb");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long total = ftell(fp) / sizeof(Subject);

  Subject subject;
  for(long i = 0; i < total; i++) {
    fseek(fp, i * sizeof(Subject), SEEK_SET);
    fread(&subject, sizeof(Subject), 1, fp);
    if(!subject.deleted) {
      printf("ID: %d, Name: %s\n", subject.subject_id, subject.name);
    }
  }

  fclose(fp);
}

void getEnrollments() {
  FILE* fp = fopen("enrollments.dat", "rb");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  long total = ftell(fp) / sizeof(Enrollment);

  Enrollment enrollment;
  for(long i = 0; i < total; i++) {
    fseek(fp, i * sizeof(Enrollment), SEEK_SET);
    fread(&enrollment, sizeof(Enrollment), 1, fp);
    if(!enrollment.deleted) {
      printf("Enrollment ID: %d, Student ID: %d, Subject ID: %d, Final Grade: %.2f\n",
             enrollment.enrollment_id, enrollment.student_id, enrollment.subject_id, enrollment.final_grade);
    }
  }

  fclose(fp);
}

void menu() {
  char* currEntity = NULL;
  while(1) {
    int currOperation = -1;
    int hasCurrentEntity = currEntity != NULL;

    char* lastOption = !hasCurrentEntity ? "EXIT" : "Return to main menu";
    if(hasCurrentEntity) {
      printf("\n--- MENU: Select one operation for %s ---\n", currEntity);
      printf("1 - Add %s\n", currEntity);
      printf("2 - List %s\n", currEntity);
      printf("3 - Remove %s\n", currEntity);
      if(strcmp(currEntity, "Enrollments") == 0) {
        printf("4 - Update Enrollment Grade\n");
      }
      printf("0 - %s\n", lastOption);
    } else {
      printf("\n--- MAIN MENU ---\n");
      printf("1 - Students\n");
      printf("2 - Subjects\n");
      printf("3 - Enrollments\n");
      printf("0 - EXIT\n");
    }

    printf("Select option: ");
    if(scanf("%d", &currOperation) != 1) {
      break;
    }
    printf("\n");

    if(!hasCurrentEntity) {
      switch(currOperation) {
        case 1:
          currEntity = "Students";
          break;
        case 2:
          currEntity = "Subjects";
          break;
        case 3:
          currEntity = "Enrollments";
          break;
        case 0:
          exit(0);
        default:
          printf("Invalid option\n");
          break;
      }
    } else {
      switch(currOperation) {
        case 1: {
          int id;
          char name[30];
          float grade;
          if(strcmp(currEntity, "Students") == 0) {
            printf("Student ID: ");
            scanf("%d", &id);
            printf("Name: ");
            scanf(" %29[^\n]", name);
            addStudent(id, name);
          } else if(strcmp(currEntity, "Subjects") == 0) {
            printf("Subject ID: ");
            scanf("%d", &id);
            printf("Name: ");
            scanf(" %29[^\n]", name);
            addSubject(id, name);
          } else if(strcmp(currEntity, "Enrollments") == 0) {
            int student_id, subject_id;
            printf("Enrollment ID: ");
            scanf("%d", &id);
            printf("Student ID: ");
            scanf("%d", &student_id);
            printf("Subject ID: ");
            scanf("%d", &subject_id);
            printf("Final Grade: ");
            scanf("%f", &grade);
            addEnrollment(id, student_id, subject_id, grade);
          }
          break;
        }
        case 2:
          if(strcmp(currEntity, "Students") == 0) {
            getStudents();
          } else if(strcmp(currEntity, "Subjects") == 0) {
            getSubjects();
          } else if(strcmp(currEntity, "Enrollments") == 0) {
            getEnrollments();
          }
          break;
        case 3: {
          int id;
          printf("Enter ID to remove: ");
          scanf("%d", &id);

          if(strcmp(currEntity, "Students") == 0) {
            long offset = btreeGetOffset(studentsIndex, id);
            if(offset == -1) {
              printf("Student ID %d not found.\n", id);
            } else {
              FILE* fp = fopen("students.dat", "rb+");
              if(!fp) break;
              Student student;
              fseek(fp, offset * sizeof(Student), SEEK_SET);
              fread(&student, sizeof(Student), 1, fp);
              student.deleted = 1;
              fseek(fp, offset * sizeof(Student), SEEK_SET);
              fwrite(&student, sizeof(Student), 1, fp);
              fclose(fp);
              removeEnrollmentsByStudent(id);
              studentsIndex = removeBTree(studentsIndex, id);
              saveBTreeToDisk(studentsIndex, "students.idx");
              printf("Student removed.\n");
            }

          } else if(strcmp(currEntity, "Subjects") == 0) {
            long offset = btreeGetOffset(subjectsIndex, id);
            if(offset == -1) {
              printf("Subject ID %d not found.\n", id);
            } else {
              FILE* fp = fopen("subjects.dat", "rb+");
              if(!fp) break;
              Subject subject;
              fseek(fp, offset * sizeof(Subject), SEEK_SET);
              fread(&subject, sizeof(Subject), 1, fp);
              subject.deleted = 1;
              fseek(fp, offset * sizeof(Subject), SEEK_SET);
              fwrite(&subject, sizeof(Subject), 1, fp);
              fclose(fp);
              removeEnrollmentsBySubject(id);
              subjectsIndex = removeBTree(subjectsIndex, id);
              saveBTreeToDisk(subjectsIndex, "subjects.idx");
              printf("Subject removed.\n");
            }

          } else if(strcmp(currEntity, "Enrollments") == 0) {
            long offset = btreeGetOffset(enrollmentsIndex, id);
            if(offset == -1) {
              printf("Enrollment ID %d not found.\n", id);
            } else {
              FILE* fp = fopen("enrollments.dat", "rb+");
              if(!fp) break;
              Enrollment enrollment;
              fseek(fp, offset * sizeof(Enrollment), SEEK_SET);
              fread(&enrollment, sizeof(Enrollment), 1, fp);
              enrollment.deleted = 1;
              fseek(fp, offset * sizeof(Enrollment), SEEK_SET);
              fwrite(&enrollment, sizeof(Enrollment), 1, fp);
              fclose(fp);
              enrollmentsIndex = removeBTree(enrollmentsIndex, id);
              saveBTreeToDisk(enrollmentsIndex, "enrollments.idx");
              printf("Enrollment removed.\n");
            }
          }
          break;
        }
        case 4:
          if(strcmp(currEntity, "Enrollments") == 0) {
            int id;
            float grade;
            printf("Enrollment ID to update: ");
            scanf("%d", &id);
            printf("New Final Grade: ");
            scanf("%f", &grade);
            updateEnrollmentGrade(id, grade);
          } else {
            printf("Invalid option\n");
          }
          break;
        case 0:
          currEntity = NULL;
          break;
        default:
          printf("Invalid option\n");
          break;
      }
    }
  }
}

int main() {
  initializeIndexes();
  menu();
  return 0;
}
