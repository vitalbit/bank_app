#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

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
