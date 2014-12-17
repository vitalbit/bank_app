#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#include "header.h"

int debitMoney(sqlite3 *db, double amount, char *operationDate, int clientID, int accountID)
{
	char *operationName;

	double balance;
	int operationID;
	int exitCode;

	sqlite3_stmt *statement;

	const char *selectBalance="select balance from Account where client_id=? and account_id=?";
	const char *updateBalance="update Account set balance=? where client_id=? and account_id=?";
	const char *insertOperation="insert into Operation(operation_name) values(?)";
	const char *selectOperationID="select operation_id from Operation where operation_name=? limit 1";
	const char *selectOperationIDLast="select operation_id from Operation Order by operation_id DESC limit 1";
	const char *insertLOG="insert into Log(log_date, operation_id, account_id) values(?,?,?)";

	operationName="debit";

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

	exitCode=sqlite3_step(statement);

	if(exitCode==SQLITE_DONE)
	{
		printf("Wrong accountId or clientId!\n");
		return -1;
	}
	else if(exitCode==SQLITE_ROW)
		balance=sqlite3_column_double(statement, 0);	

	if(balance<amount)
	{
		printf("It's not enough money on your account!!\n");
		return -1;
	}

	if(sqlite3_prepare(db, updateBalance, -1, &statement, 0)!= SQLITE_OK) 
	{
		printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	if (sqlite3_bind_double(statement, 1, balance - amount))
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

	exitCode=sqlite3_step(statement);

	if(exitCode==SQLITE_ROW)
		operationID=sqlite3_column_int(statement, 0);	
	else if(exitCode==SQLITE_DONE)
	{
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

		if(sqlite3_prepare(db, selectOperationIDLast, -1, &statement, 0)!=SQLITE_OK) 
		{
			printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
			return -1;
		}

		if(sqlite3_step(statement)==SQLITE_ROW)
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
		printf("LOG table updated!!\n");

	sqlite3_close(db);
	return 0;
}

int getUserInfo(sqlite3 *db, int clientID)
{
	char *email;
	char *fullName;
	char *nickName;
	char *password;

	int block;
	int exitCode;

	sqlite3_stmt *statement;

	const char *selectClient="select * from Client where client_id=?";

	if(sqlite3_prepare(db, selectClient, -1, &statement, 0)!= SQLITE_OK) 
	{
		printf("Could not prepare statement: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	if (sqlite3_bind_int(statement, 1, clientID))
	{
		printf("Could not bind text: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	
	exitCode=sqlite3_step(statement);

	if(exitCode==SQLITE_ROW)
	{
		fullName=(char*)sqlite3_column_text(statement, 1);
		printf("Full name: %s\n", fullName);

		email=(char*)sqlite3_column_text(statement, 2);
		printf("E-mail: %s\n", email);

		nickName=(char*)sqlite3_column_text(statement, 3);
		printf("Nick: %s\n", nickName);

		password=(char*)sqlite3_column_text(statement, 4);
		printf("Password: %s\n", password);

		block=sqlite3_column_int(statement, 5);
		printf("Is blocked: %d\n", block);
	}
	else if(exitCode==SQLITE_DONE)
		printf("Wrong client id!\n");

	sqlite3_finalize(statement);

	sqlite3_close(db);
}
