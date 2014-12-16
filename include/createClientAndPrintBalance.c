#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"


int createNewClient(sqlite3 *db, char *full_name, char *email, char *nickname, char *password){
	const char *insertClient = "insert into Client(full_name, email, nickname, password, is_block, is_delete) values(?,?,?,?,?,?)";
	const char *selectClient = "select client_id from Client where nickname=?";
	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, selectClient, -1, &statement, 0) != SQLITE_OK)
	{
		printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	if (sqlite3_bind_text(statement, 1, nickname, strlen(nickname), SQLITE_STATIC))
	{
		printf("Could not bind text: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	if (sqlite3_step(statement) != SQLITE_ROW){

		if (sqlite3_prepare(db, insertClient, -1, &statement, 0) != SQLITE_OK)
		{
			printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
			return -1;
		}

		if (sqlite3_bind_text(statement, 1, full_name, strlen(full_name), SQLITE_STATIC))
		{
			printf("Could not bind text: %s\n", sqlite3_errmsg(db));
			return -1;
		}

		if (sqlite3_bind_text(statement, 2, email, strlen(email), SQLITE_STATIC))
		{
			printf("Could not bind text: %s\n", sqlite3_errmsg(db));
			return -1;
		}

		if (sqlite3_bind_text(statement, 3, nickname, strlen(nickname), SQLITE_STATIC))
		{
			printf("Could not bind text: %s\n", sqlite3_errmsg(db));
			return -1;
		}
		if (sqlite3_bind_text(statement, 4, password, strlen(password), SQLITE_STATIC))
		{
			printf("Could not bind text: %s\n", sqlite3_errmsg(db));
			return -1;
		}
		if (sqlite3_bind_int(statement, 5, 0))
		{
			printf("Could not bind int: %s\n", sqlite3_errmsg(db));
			return -1;
		}
		if (sqlite3_bind_int(statement, 6, 0))
		{
			printf("Could not bind int: %s\n", sqlite3_errmsg(db));
			return -1;
		}

		if (sqlite3_step(statement) == SQLITE_DONE)
		{
			printf("Client created.\n");
		}
	}
	else {
		printf("Client with current login already exists.\n");
	}
}

void printAccountBalance(sqlite3 *db, int account_id){
	double balance;
	const char *selectBalance = "select balance from Account where account_id=?";

	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, selectBalance, -1, &statement, 0) != SQLITE_OK)
	{
		printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
		return;
	}

	if (sqlite3_bind_int(statement, 1, account_id))
	{
		printf("Could not bind int: %s\n", sqlite3_errmsg(db));
		return;
	}

	if (sqlite3_step(statement) == SQLITE_ROW){
		balance = sqlite3_column_double(statement, 0);
		printf("Balance = %f\n", balance);
	}
	else {
		printf("No such account\n");
		return;
	}
}
