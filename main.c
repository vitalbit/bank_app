#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/sqlite3.h"
#include <time.h>

#include "include/header.h"

#define false 0
#define true 1
typedef int bool; // or #define bool int

// Functionality block

char *getRole(sqlite3 *db, int curr_id)
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

void deleteClientByClientID(sqlite3 *db, int client_id){
  sqlite3_stmt *stmt = NULL;
  char *sqlDeleteClientByClientID = "UPDATE Client SET is_delete = 1 WHERE client_id = ?";
  sqlite3_prepare_v2(db, sqlDeleteClientByClientID, strlen(sqlDeleteClientByClientID), &stmt, NULL);
  sqlite3_bind_int(stmt, 1, client_id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

bool authorization(sqlite3 *db, char nick[100], char password[100])
{
  char query[500] = "select role_id from User where nickname = '";
  int curr_id = 0;
  sqlite3_stmt *statement;
  strcat(query, nick);
  strcat(query, "' and password = '");
  strcat(query, password);
  strcat(query, "';");
  sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
  if (sqlite3_step(statement) == SQLITE_ROW)
  {
    curr_id = atoi((char*)sqlite3_column_text(statement, 0));
  }
  return curr_id;
}
// end

int main(int argc, char **argv) {
  sqlite3 *db;
  char *zErrMsg = 0;
  char buffer[50], nick[100], pass[100];
  const int OPERATION_COUNT = 12;
  int rc, operation, id;
  char *nickname = "";
  int account_id = 0;
  int client_id = 0;
  int amount = 0;
  char *operationDate="";
  char password[80];
  char client_full_name[80];
  char client_email[80];
  char client_nickname[80];
  int length = 0;
  int editFieldNum = 0;
  int account_type = 1;
  int curr_id = 0;
  double credit_sum = 0;

  // Put our operation here
  char *states[12] = {
    "1. See all account.",
    "2. Credit money.",
    "3. Block account",
    "4. View the history of user operations",
    "5. Unblock account",
    "6. Delete client (by client id)",
    "7. Debit money",
    "8. Get user info",
    "9. Create client",
    "10. Check balance",
    "11. Edit client information",
    "12. Add account to client"
  };

  rc = sqlite3_open(argv[1], &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  
  printf("\tEnter your nickname: ");
  scanf("%s", &nick);
  printf("\tEnter your password: ");
  scanf("%s", &pass);
  curr_id = authorization(db, nick, pass);

  if (curr_id > 0) {
    char *role = getRole(db, curr_id);

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
			printf("\tEnter account id: ");
			scanf("%d", &account_id);
			printf("\tEnter credit sum: ");
			scanf("%lf", &credit_sum);
			if (credit(db, account_id, credit_sum))
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
			printf("Enter client id\n");
			scanf("%s", &client_id);
			printf("Enter account  id\n");
			scanf("%u", &account_id);
			debitMoney(db, amount, operationDate, client_id, account_id);
		}
		else
			printf("Your are not an Operator\n");
        break;
      case 8:
		if (strcmp(role, "Operator") == 0 || strcmp(role, "Administrator") == 0)
		{
			printf("Enter client id\n");
			scanf("%s", &client_id);
			getUserInfo(db, client_id);
		}
		else
			printf("Your are not Operator or Administrator\n");
        break;
	  case 9:
		  if (strcmp(role, "Administrator") == 0)
		  {
			  printf("Enter new client full name (first and last names):\n");
			  fflush(stdin);
			  fgets(client_full_name, 79, stdin);
			  printf("Enter new client email:\n");
			  fflush(stdin);
			  fgets(client_email, 79, stdin);
			  printf("Enter new client nickname:\n");
			  fflush(stdin);
			  fgets(client_nickname, 79, stdin);
			  printf("Enter new client password:\n"); 
			  fflush(stdin);
			  fgets(password, 79, stdin);
			  createNewClient(db, &client_full_name, &client_email, &client_nickname, &password);
		  }
		  else
			  printf("Your are not an Administrator\n");
		  break;
	  case 10:
		  if (strcmp(role, "Operator") == 0 || strcmp(role, "Administrator") == 0)
		  {
			  printf("Enter client's account id:\n");
			  scanf("%d", &account_id);
			  printAccountBalance(db, account_id);
		  }
		  else
			  printf("Your are not Operator or Administrator\n");
		  break;
	case 11:
		if (strcmp(role, "Administrator") == 0)
		  {
		  	printf("Input client's ID: ");
			scanf("%d", &client_id);
			printf("Select field to edit:\n\t1 - Full name\n\t2 - Nickname\n\t3 - Email\n\t0 - Exit\n");
			scanf("%d", &editFieldNum);
			editClient(db, client_id, editFieldNum);
			break;
		  }
		else
			 printf("Your are not Administrator\n");
		break;
	
	case 12:
		if (strcmp(role, "Administrator") == 0)
		  {
		  	printf("Input client's ID: ");
			scanf("%d", &client_id);
			printf("Select account type to edit:\n\t1 - Savings acc\n\t2 - Current acc\n\t3 - Overdraft acc\n");
			scanf("%d", &account_type);
			addAccountToClient(db, client_id, account_type);
			break;
		  }
		else
			 printf("Your are not Administrator\n");	
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
