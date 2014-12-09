#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/sqlite3.h"
#include "checkAccountBlock.h"

void creditLog(sqlite3 *db, int account_id)
{
  char query[255] = "insert into Log values (null, '";
  sqlite3_stmt *statement;
  time_t t = time(NULL);
  struct tm* tm = localtime(&t);
  char date[100] = "", tmp[100] = "";
  strcat(date, sprintf(tmp, "%d", tm->tm_year + 1900));

  strcat(date, "-");
  strcat(date, sprintf(tmp, "%d", tm->tm_mon + 1));

  strcat(date, "-");
  strcat(date, sprintf(tmp, "%d", tm->tm_mday));

  strcat(query, date);
  strcat(query, "', 3, ");
  sprintf(tmp, "%d", account_id);

  strcat(query, tmp);
  strcat(query, ");");
  sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
  sqlite3_step(statement);
  sqlite3_finalize(statement);
}

bool credit(sqlite3 *db)
{
    int account_id = 0;
    double credit_sum = 0;
    char *query = "select at.name, a.balance from AccountType at inner join Account a on a.account_type_id = at.account_type_id where a.account_id = ?;";
    sqlite3_stmt *statement;
    printf("\tEnter account id: ");
    scanf("%d", &account_id);
    printf("\tEnter credit sum: ");
    scanf("%lf", &credit_sum);
    if (!isAccountBlock(db, account_id))
    {
      sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
      sqlite3_bind_int(statement, 1, account_id);
      if (sqlite3_step(statement) == SQLITE_ROW)
      {
        char *typeAcc = (char*)sqlite3_column_text(statement, 0);
        char *balance = (char*)sqlite3_column_text(statement, 1);
        if (strcmp(typeAcc, "Saving account") == 0)
        {
          sqlite3_stmt *statement2;
          char *query2 = "update account set balance = ? where account_id = ?;";
          int res;
          sqlite3_prepare_v2(db, query2, strlen(query2), &statement2, NULL);
          sqlite3_bind_double(statement2, 1, atof(balance) + credit_sum);
          sqlite3_bind_int(statement2, 2, account_id);
          res = sqlite3_step(statement2);
          sqlite3_finalize(statement2);
          sqlite3_close(db);
          creditLog(db, account_id);
          return true;
        }
        else if (strcmp(typeAcc, "Current account") == 0)
        {
          sqlite3_stmt *statement2;
          char *query2 = "update account set balance = ? where account_id = ?;";
          int res;
          sqlite3_stmt *statement3;
          char *query3 = "select total_operations from CurrentAccount where account_id = ?;";
          sqlite3_prepare_v2(db, query2, strlen(query2), &statement2, NULL);
          sqlite3_bind_double(statement2, 1, atof(balance) + credit_sum);
          sqlite3_bind_int(statement2, 2, account_id);
          res = sqlite3_step(statement2);
          sqlite3_finalize(statement2);

          sqlite3_prepare_v2(db, query3, strlen(query3), &statement3, NULL);
          sqlite3_bind_int(statement3, 1, account_id);
          if (sqlite3_step(statement3) == SQLITE_ROW)
          {
            int total = atoi((char*)sqlite3_column_text(statement3, 0));
            sqlite3_stmt *statement4;
            char *query4 = "update CurrentAccount set total_operations = ? where account_id = ?;";
            int res;
            sqlite3_prepare_v2(db, query4, strlen(query4), &statement4, NULL);
            sqlite3_bind_int(statement4, 1, total + 1);
            sqlite3_bind_int(statement4, 2, account_id);
            res = sqlite3_step(statement4);
            sqlite3_finalize(statement4);
            sqlite3_close(db);
            creditLog(db, account_id);
            return true;
          }
        }
        else if (strcmp(typeAcc, "Overdraft account") == 0)
        {
          sqlite3_stmt *statement2;
          char *query2 = "select max_amount from OverdraftAccount where account_id = ?;";
          sqlite3_prepare_v2(db, query2, strlen(query2), &statement2, NULL);
          sqlite3_bind_int(statement2, 1, account_id);
          if (sqlite3_step(statement2) == SQLITE_ROW)
          {
            double max = atof((char*)sqlite3_column_text(statement2, 0));
            if (atof(balance) + credit_sum <= max)
            {
              sqlite3_stmt *statement3;
              char *query3 = "update account set balance = ? where account_id = ?;";
              int res;
              sqlite3_prepare_v2(db, query3, strlen(query3), &statement3, NULL);
              sqlite3_bind_double(statement3, 1, atof(balance) + credit_sum);
              sqlite3_bind_int(statement3, 2, account_id);
              res = sqlite3_step(statement3);
              sqlite3_finalize(statement3);
              sqlite3_close(db);
              creditLog(db, account_id);
              return true;
            }
            else
              printf("\tResult balance more than limit!\n");
          }
        }
      }
      else
        printf("\tNo account with such id!\n");
    }
    else
      printf("\tAccount is block!\n");
  return false;
}