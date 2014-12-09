#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>

#include "authorization.h"

int curr_id_ = -1;

bool authorization(sqlite3 *db)
{
  char nick[100], password[100];
  char query[1000] = "select role_id from User where nickname = '";
  sqlite3_stmt *statement;
  printf("\tEnter your nickname: ");
  scanf("%s", nick);
  printf("\tEnter your password: ");
  scanf("%s", password);
  strcat(query, nick);
  strcat(query, "' and password = '");
  strcat(query, password);
  strcat(query, "';");
  sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
  if (sqlite3_step(statement) == SQLITE_ROW)
  {
    curr_id_ = atoi((char*)sqlite3_column_text(statement, 0));
    return true;
  }
  return false;
}