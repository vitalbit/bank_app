#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "accountOperation.h"

int isAccountBlock(sqlite3 *db, int account_id)
{
  sqlite3_stmt *stmt = NULL;
  char *sqlBlockAccountByAccountID = "select is_block from Account WHERE account_id = ?;";
  sqlite3_prepare_v2(db, sqlBlockAccountByAccountID, strlen(sqlBlockAccountByAccountID), &stmt, NULL);
  sqlite3_bind_int(stmt, 1, account_id);
  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    return atoi((char*)sqlite3_column_text(stmt, 0));
  }
  else
    return 1;
}

void blockAccountByAccountID(sqlite3 *db, int account_id){
  sqlite3_stmt *stmt = NULL;
  char *sqlBlockAccountByAccountID = "UPDATE Account SET is_block = 1 WHERE account_id = ?";
  sqlite3_prepare_v2(db, sqlBlockAccountByAccountID, strlen(sqlBlockAccountByAccountID), &stmt, NULL);
  sqlite3_bind_int(stmt, 1, account_id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void unblockAccountByAccountID(sqlite3 *db, int account_id){
  sqlite3_stmt *stmt = NULL;
  char *sqlBlockAccountByAccountID = "UPDATE Account SET is_block = 0 WHERE account_id = ?";
  sqlite3_prepare_v2(db, sqlBlockAccountByAccountID, strlen(sqlBlockAccountByAccountID), &stmt, NULL);
  sqlite3_bind_int(stmt, 1, account_id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

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
