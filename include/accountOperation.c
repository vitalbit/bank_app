#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "accountOperation.h"

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