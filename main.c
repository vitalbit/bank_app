#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>

#include "include/header.h"

#define false 0
#define true 1
typedef int bool; // or #define bool int

int curr_id = -1;


// Functionality block

char *getRole(sqlite3 *db)
{
  char* query = "select role_name from User_role where role_id = ?;";
  sqlite3_stmt *statement;
  sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
  sqlite3_bind_int(statement, 1, curr_id);
  if (sqlite3_step(statement) == SQLITE_ROW)
  {
    return (char*)sqlite3_column_text(statement, 0);
  }
  return NULL;
}

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

void deleteClientByClientID(sqlite3 *db, int client_id){
  sqlite3_stmt *stmt = NULL;
  char *sqlDeleteClientByClientID = "UPDATE Client SET is_delete = 1 WHERE client_id = ?";
  sqlite3_prepare_v2(db, sqlDeleteClientByClientID, strlen(sqlDeleteClientByClientID), &stmt, NULL);
  sqlite3_bind_int(stmt, 1, client_id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

int debitMoney(sqlite3 *db, int amount, char *operationDate, char *clientNickName, char *clientPassword, int accountID)
{
  char *operationName;

  double balance;
  int clientID;
  int operationID;

  sqlite3_stmt *statement;

  const char *selectClient="select client_id from Client where nickname=? and password=?";
  const char *selectBalance="select balance from Account where client_id=? and account_id=?";
  const char *updateBalance="update Account set balance=? where client_id=? and account_id=?";
  const char *insertOperation="insert into Operation(operation_name) values(?)";
  const char *selectOperationID="select operation_id from Operation where operation_name=? limit 1";
  const char *insertLOG="insert into Log(log_date, operation_id, account_id) values(?,?,?)";

  operationName="debit";

  if(sqlite3_prepare(db, selectClient, -1, &statement, 0)!= SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 1, clientNickName, strlen(clientNickName), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 2, clientPassword, strlen(clientPassword), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  while(sqlite3_step(statement)==SQLITE_ROW)
  {
    clientID=sqlite3_column_int(statement, 0);

    if(!clientID)
    {
      printf("Wrong nickname or password!!\n");
      return -1;
    }
  }

  sqlite3_finalize(statement);

  if(sqlite3_prepare(db, selectBalance, -1, &statement, 0)!= SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_int(statement, 1, clientID))
  {
    printf("Could not bind integer: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_int(statement, 2, accountID))
  {
    printf("Could not bind integer: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  while(sqlite3_step(statement)==SQLITE_ROW)
  {
    balance=sqlite3_column_double(statement, 0);    
  }
  
  if(sqlite3_prepare(db, updateBalance, -1, &statement, 0)!= SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_double(statement, 1, balance + amount))
  {
    printf("Could not bind double: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_int(statement, 2, clientID))
  {
    printf("Could not bind integer: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_int(statement, 3, accountID))
  {
    printf("Could not bind integer: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if(sqlite3_step(statement)==SQLITE_DONE)
  {
    printf("Balance updated!! You've debit %.3lf $\n", amount);
  }

  if(sqlite3_prepare(db, insertOperation, -1, &statement, 0)!=SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 1, operationName, strlen(operationName), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if(sqlite3_step(statement)==SQLITE_DONE)
  {
    printf("Row into Operation table inserted!!\n");
  }

  if(sqlite3_prepare(db, selectOperationID, -1, &statement, 0)!= SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 1, operationName, strlen(operationName), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  while(sqlite3_step(statement)==SQLITE_ROW)
  {
    operationID=sqlite3_column_int(statement, 0);   
  }
  
  if(sqlite3_prepare(db, insertLOG, -1, &statement, 0)!= SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 1, operationDate, strlen(operationDate), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_int(statement, 2, operationID))
  {
    printf("Could not bind integer: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_int(statement, 3, accountID))
  {
    printf("Could not bind integer: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if(sqlite3_step(statement)==SQLITE_DONE)
  {
    printf("LOG table updated!!\n");
  }
  return 0;
}

int getUserInfo(sqlite3 *db, char *clientNickName, char *clientPassword)
{
  char *email;
  char *fullName;

  int block;
  int clientID;
  sqlite3_stmt *statement;

  const char *selectClient="select * from Client where nickname=? and password=?";

  if(sqlite3_prepare(db, selectClient, -1, &statement, 0)!= SQLITE_OK) 
  {
    printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 1, clientNickName, strlen(clientNickName), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  if (sqlite3_bind_text(statement, 2, clientPassword, strlen(clientPassword), SQLITE_STATIC))
  {
    printf("Could not bind text: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  while(sqlite3_step(statement)==SQLITE_ROW)
  {
    clientID=sqlite3_column_int(statement, 0);

    if(!clientID)
    {
      printf("Wrong nickname or password!!\n");
      return -1;
    }
    else
    {
      printf("Client id: %d\n", clientID);
      fullName=(char*)sqlite3_column_text(statement, 1);
      printf("Full name: %s\n", fullName);
      email=(char*)sqlite3_column_text(statement, 2);
      printf("Full name: %s\n", email);
      block=sqlite3_column_int(statement, 5);
      printf("Is blocked: %d\n", block);
    }
  }

  sqlite3_finalize(statement);
}


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
    curr_id = atoi((char*)sqlite3_column_text(statement, 0));
    return true;
  }
  return false;
}
// end

int main(int argc, char **argv) {
  sqlite3 *db;
  char *zErrMsg = 0;
  char buffer[50];
  const int OPERATION_COUNT = 8;
  int rc, operation, id;
  char *nickname = "";
  int account_id = 0;
  int client_id = 0;
  int amount = 0;
  char* operationDate="";
  char* password = "";

  // Put our operation here
  char *states[OPERATION_COUNT] = {
    "1. See all account.",
    "2. Credit money.",
    "3. Block account",
    "4. View the history of user operations",
    "5. Unblock account",
    "6. Delete client (by client id)",
    "7. Debit money",
    "8. Get user info"
  };

  rc = sqlite3_open(argv[1], &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }

  if (authorization(db)) {
    char *role = getRole(db);

    while (!rc) {
      int i;
      for (i = 0; i != OPERATION_COUNT; i++) {
        printf("%s\n", states[i]);
      }
      scanf("%d", &operation);
      switch (operation) {
      case 1:
	    if (strcmp(role, "Operator") == 0 || strcmp(role, "Administrator") == 0)
			rc = getAccountInfoById(db, zErrMsg);
		else
			printf("Your are not Operator or Administrator\n");
        break;
      case 2:
		if (strcmp(role, "Operator") == 0)
		{
			if (credit(db))
				printf("Credit success!\n");
			else
				printf("Credit error!\n");
		}
		else
			printf("Your are not an Operator\n");
        break;
      case 3:
		if (strcmp(role, "Operator") == 0 || strcmp(role, "Administrator") == 0)
		{
			printf("Enter account id\n");
			scanf("%u", &account_id);
			blockAccountByAccountID(db, account_id);
		}
		else
			printf("Your are not Operator or Administrator\n");
        break;
      case 4:
		if (strcmp(role, "Administrator") == 0)
		{
			printf("Enter client account id\n");
			scanf("%u", &account_id);
			getHistoryByAccountID(db,account_id);
		}
		else
			printf("Your are not an Administrator\n");
        break;
      case 5:
		if (strcmp(role, "Operator") == 0 || strcmp(role, "Administrator") == 0)
		{
			printf("Enter account id\n");
			scanf("%u", &account_id);
			unblockAccountByAccountID(db, account_id);
		}
		else
			printf("Your are not Operator or Administrator\n");
        break;
      case 6:
		if (strcmp(role, "Administrator") == 0)
		{
			printf("Enter client id\n");
			scanf("%u", &client_id);
			deleteClientByClientID(db, client_id);
		}
		else
			printf("Your are not an Administrator\n");
        break;
      case  7:
		if (strcmp(role, "Operator") == 0)
		{
			printf("Enter amount:\n");
			scanf("%u", &amount);
			printf("Enter operation date:\n");
			scanf("%s", &operationDate);
			printf("Enter client nickname\n");
			scanf("%s", &nickname);
			printf("Enter client password\n");
			scanf("%s", &password);
			printf("Enter account  id\n");
			scanf("%u", &account_id);
			debitMoney(db, amount, operationDate, nickname, password, account_id);
		}
		else
			printf("Your are not an Operator\n");
        break;
      case 8:
		if (strcmp(role, "Operator") == 0 || strcmp(role, "Administrator") == 0)
		{
			printf("Enter client nickname\n");
			scanf("%s", &nickname);
			printf("Enter client password\n");
			scanf("%s", &password);
			getUserInfo(db, nickname, password);
		}
		else
			printf("Your are not Operator or Administrator\n");
        break;
      }

      if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
      }
    }
  }
  else
    printf("Wrong nickname or password!\n");
  sqlite3_close(db);
  return 0;
}
