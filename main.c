#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

static int printResult(void *data, int argc, char **argv, char **azColName) {
  int i;
  fprintf(stderr, "%s: ", (const char*)data);
  for (i = 0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char **argv) {
  sqlite3 *db;
  char *zErrMsg = 0;
  char buffer [50];
  char *name,*email,*pass,*info,*title,*url,*genre;
  int rc;
  int operation, id;

  char *states[3] = {
    "1. See all users.",
    "2. See all authors",
    "3. See all books"
  };
  char *sql[3] = {
    "select * from users",
    "select * from authors",
    "select * from books"
  };

  rc = sqlite3_open(argv[1], &db);
  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  while( !rc ) {
    for (int i = 0; i != 3; i++) {
      printf("%s\n", states[i]);
    }
    scanf("%d", &operation);
    switch(operation) {
      case 1:
        rc = sqlite3_exec(db, sql[0], printResult, 0, &zErrMsg);
        break;
      case 2:
        rc = sqlite3_exec(db, sql[1], printResult, 0, &zErrMsg);
        break;
      case 3:
        rc = sqlite3_exec(db, sql[2], printResult, 0, &zErrMsg);
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