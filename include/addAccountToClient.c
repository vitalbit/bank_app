#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

void addAccountToClient(sqlite3 *db, int client_id, int acc_type_id)
{
  sqlite3_stmt *stmt;
  	double balance;
  	double interest_rate;
  	char date[50];
  	char *query;
  	char *account_query;
  	int monthly_quota;
  	int total_operations;
  	double per_transaction_fee;
  	double max_overdraft_amount;
  	int max_overdraft_term;
  	double overdraft_penalty;
  
  		printf("Input start balance: ");
  		scanf("%lf", &balance);
  		printf("Input open date (yyyy-mm-dd): ");
  		scanf("%s", date);
  
  		query = "insert into Account (balance, account_type_id, client_id, start_date, close_date, is_block) values (?,?,?,?,?,?)";
  		sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
  		sqlite3_bind_double(stmt, 1, balance);
  		sqlite3_bind_int(stmt, 2, acc_type_id);
  		sqlite3_bind_int(stmt, 3, client_id);
  		sqlite3_bind_text(stmt, 4, date, strlen(date), SQLITE_STATIC);
  		sqlite3_bind_text(stmt, 5, "", 0, SQLITE_STATIC);
  		sqlite3_bind_int(stmt, 6, 0);
  		sqlite3_step(stmt);
  		sqlite3_finalize(stmt);
  
  		switch (acc_type_id)
  		{
  		case 1:
  			printf("Input interest rate: ");
  			scanf("%lf",&interest_rate);
  			
  			account_query = "insert into SavingAccount (interest_rate, account_id) values (?,(SELECT COUNT (*) FROM Account))";
  			sqlite3_prepare_v2(db, account_query, strlen(account_query), &stmt, NULL);
  			sqlite3_bind_double(stmt, 1, interest_rate);
  			if (sqlite3_step(stmt) == SQLITE_DONE)
  				printf("Savings Account added succesfully.\n");
  			else
  				printf(sqlite3_errmsg(db));
  			sqlite3_finalize(stmt);
  			break;
  			
  		case 2:
  			printf("Input monthly quota: ");
  			scanf("%d", &monthly_quota);
  			printf("Input total operations number: ");
  			scanf("%d", &total_operations);
  			printf("Input per transactions fee: ");
  			scanf("%lf", &per_transaction_fee);
  
  			account_query = "insert into CurrentAccount (monthly_quota, total_operations, account_id, per_transaction_fee) values (?,?,(SELECT COUNT (*) FROM Account),?)";
  			sqlite3_prepare_v2(db, account_query, strlen(account_query), &stmt, NULL);
  			sqlite3_bind_int(stmt, 1, monthly_quota);
  			sqlite3_bind_int(stmt, 2, total_operations);
  			sqlite3_bind_double(stmt, 3, per_transaction_fee);
  			
  			if (sqlite3_step(stmt) == SQLITE_DONE)
  				printf("Current Account added succesfully.\n");
  			else
  				printf(sqlite3_errmsg(db));
  			sqlite3_finalize(stmt);
  			break;
  			
  		case 3:
  			printf("Input maximum amount: ");
  			scanf("%lf", &max_overdraft_amount);
  			printf("Input overdraft term: ");
  			scanf("%d", &max_overdraft_term);
  			printf("Input penalty percentage: ");
  			scanf("%lf", &overdraft_penalty);
  
  			account_query = "insert into OverdraftAccount (max_amount, max_term, penalty, account_id) values (?,?,?,(SELECT COUNT (*) FROM Account))";
  			sqlite3_prepare_v2(db, account_query, strlen(account_query), &stmt, NULL);
  			sqlite3_bind_int(stmt, 1, max_overdraft_amount);
  			sqlite3_bind_int(stmt, 2, max_overdraft_term);
  			sqlite3_bind_double(stmt, 3, overdraft_penalty);
  			
  			if (sqlite3_step(stmt) == SQLITE_DONE)
  				printf("Current Account added succesfully.\n");
  			else
  				printf(sqlite3_errmsg(db));
  			sqlite3_finalize(stmt);
  			break;
  
  		default:
  			break;
  		}
}
