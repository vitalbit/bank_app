#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define false 0
#define true 1
typedef int bool; // or #define bool int

int curr_id = -1;
char *role = "";

// Main print
static int printResult(void *data, int argc, char **argv, char **azColName) {
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

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

int getAccountInfoById(sqlite3 *db, char *errmsg) {
	char id[10];
	char sql[100] = "select * from account where account_id=";

	printf("\tEnter account id to see: ");
	scanf("%s", id);

	strcat(sql, id);
	return sqlite3_exec(db, sql, printResult, 0, &errmsg);
}

bool credit(sqlite3 *db)
{
	if (strcmp(role, "Administrator") == 0 || strcmp(role, "Operator") == 0)
	{
		int account_id = 0;
		double credit_sum = 0;
		char *query = "select at.name, a.balance from AccountType at inner join Account a on a.account_type_id = at.account_type_id where a.account_id = ?;";
		sqlite3_stmt *statement;
		printf("\tEnter account id: ");
		scanf("%d", &account_id);
		printf("\tEnter credit sum: ");
		scanf("%lf", &credit_sum);
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
	return false;
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
	int OPERATION_COUNT = 2;
	int rc, operation, id;

	// Put our operation here
	char *states[2] = {
		"1. See all account.",
		"2. Credit money."
	};

	rc = sqlite3_open(argv[1], &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}

	if (authorization(db))
	{
		role = getRole(db);

		while (!rc) {
			for (int i = 0; i != OPERATION_COUNT; i++) {
				printf("%s\n", states[i]);
			}
			scanf("%d", &operation);
			switch (operation) {
			case 1:
				rc = getAccountInfoById(db, zErrMsg);
				break;
			case 2:
				if (credit(db))
					printf("Credit success!\n");
				else
					printf("Credit error!\n");
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