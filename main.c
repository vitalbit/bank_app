#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

// Main print
static int printResult(void *data, int argc, char **argv, char **azColName) {
  int i;
  fprintf(stderr, "%s: ", (const char*)data);
  for (i = 0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

// Functionality block

// end

int main(int argc, char **argv) {

  return 0;
}