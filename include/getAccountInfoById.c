#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "getAccountInfoById.h"

static int printResult(void *data, int argc, char **argv, char **azColName) {
  int i;
  fprintf(stderr, "%s: ", (const char*)data);
  for (i = 0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int getAccountInfoById(sqlite3 *db, char *errmsg) {
  char id[10];
  char sql[100] = "select * from account where account_id=";

  printf("\tEnter account id to see: ");
  scanf("%s", id);

  strcat(sql, id);
  return sqlite3_exec(db, sql, printResult, 0, &errmsg);
}
