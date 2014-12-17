#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

#define false 0
#define true 1
typedef int bool;

bool checkBlocking(sqlite3 *db, int account_id){
	sqlite3_stmt *stmt = NULL;
	char *checkBlocking = "Select is_block from Account WHERE account_id = ?";
	sqlite3_prepare_v2(db, checkBlocking, strlen(checkBlocking), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, account_id);
	sqlite3_step(stmt);
	if(sqlite3_column_int(stmt, 0) == 1)
		return true;
	else return false;
}

bool checkExistAccount(sqlite3 *db, int account_id){
	sqlite3_stmt *stmt = NULL;
	int result = 0;
	char *checkBlocking = "Select count(*) from Account WHERE account_id = ?";
	sqlite3_prepare_v2(db, checkBlocking, strlen(checkBlocking), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, account_id);
	sqlite3_step(stmt);
	if(sqlite3_column_int(stmt, 0) == 1)
		return true;
	else return false;
}

void blockAccountByAccountID(sqlite3 *db, int account_id){
	sqlite3_stmt *stmt = NULL;
	char *sqlBlockAccountByAccountID = "UPDATE Account SET is_block = 1 WHERE account_id = ?";
	if(!checkExistAccount(db,account_id)){
		printf("Check account id!\n");
		return;
	}
	sqlite3_prepare_v2(db, sqlBlockAccountByAccountID, strlen(sqlBlockAccountByAccountID), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, account_id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(checkBlocking(db, account_id)){
		printf("The account is blocked!\n");
	}else{
		printf("The account is not blocked!\n");
	}
}

void unblockAccountByAccountID(sqlite3 *db, int account_id){
	sqlite3_stmt *stmt = NULL;
	char *sqlBlockAccountByAccountID = "UPDATE Account SET is_block = 0 WHERE account_id = ?";
	if(!checkExistAccount(db,account_id)){
		printf("Check account id!\n");
		return;
	}
	sqlite3_prepare_v2(db, sqlBlockAccountByAccountID, strlen(sqlBlockAccountByAccountID), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, account_id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if(checkBlocking(db, account_id)==false){
		printf("The account is unblocked!\n");
	}else{
		printf("The account is not unblocked!\n");
	}
}
