#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

void getHistoryByAccountID(sqlite3 *db, int account_id){
  sqlite3_stmt *stmt = NULL;
  char* sqlGetHistoryByAccountID = "SELECT L.log_date , O.operation_name FROM Log L INNER JOIN Operation O ON L.operation_id = O.operation_id where L.account_id = ?";
  char * date = 0;
  char * operation = 0;
  sqlite3_prepare_v2(db, sqlGetHistoryByAccountID, strlen(sqlGetHistoryByAccountID), &stmt, NULL);
  sqlite3_bind_int(stmt, 1, account_id);

  int res = 0;
  while ((res = sqlite3_step(stmt)) == SQLITE_ROW)         
  {
    date = (char*)sqlite3_column_text(stmt, 0);
    operation = (char*)sqlite3_column_text(stmt, 1);

    printf("date : %s  operation : %s\n",date, operation);
  }
  sqlite3_finalize(stmt);
}