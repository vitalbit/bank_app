#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int getAccountInfoById(sqlite3 *db, char *errmsg) {
  char id[10];
  char sql[100] = "select * from account where account_id=";

  printf("\tEnter account id to see: ");
  scanf("%s", id);

  strcat(sql, id);
  return sqlite3_exec(db, sql, printResult, 0, &errmsg);
}
// end

int main(int argc, char **argv) {
  sqlite3 *db;
  char *zErrMsg = 0;
  char buffer [50];
  int OPERATION_COUNT = 1;
  int rc, operation, id;

  // Put our operation here
  char *states[1] = {
    "1. See all account."
  };

  rc = sqlite3_open(argv[1], &db);
  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  while( !rc ) {
    for (int i = 0; i != OPERATION_COUNT; i++) {
      printf("%s\n", states[i]);
    }
    scanf("%d", &operation);
    switch(operation) {
      case 1:
        rc = getAccountInfoById(db, zErrMsg);
        break;
    }

    if( rc!=SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
  }
  sqlite3_close(db);
  return 0;
}