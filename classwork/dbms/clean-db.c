// gcc clean-db.c -o clean && ./clean && rm -rf ./clean
#include <stdio.h>

int main() {
  remove("students.dat");
  remove("students.idx");
  remove("subjects.dat");
  remove("subjects.idx");
  remove("enrollments.dat");
  remove("enrollments.idx");

  printf("cleanEd\n\n");
  return 0;
}