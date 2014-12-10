#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

void editClient(sqlite3 *db, int client_id, int field_num)
{
	sqlite3_stmt *stmt = NULL;
	char *query;
	char full_name[80];
	char nickname[80];
	char email[80];

		switch (field_num)
		{
		case 1:
			printf("Input new full name: ");
			fflush(stdin);
			fgets(full_name, 79, stdin);
			query = "update Client set full_name = ? where client_id = ?";
			sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
			sqlite3_bind_text(stmt, 1, full_name, strlen(full_name), SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, client_id);
			if (sqlite3_step(stmt) == SQLITE_DONE)
			{
				printf("Full Name updated!! New Full Name is %s\n", full_name);
			}
			sqlite3_finalize(stmt);
			break;
		case 2:
			printf("Input new nickname: ");
			fflush(stdin);
			fgets(nickname, 79, stdin);
			query = "update Client set nickname = ? where client_id = ?";
			sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
			sqlite3_bind_text(stmt, 1, nickname, strlen(nickname), SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, client_id);
			if (sqlite3_step(stmt) == SQLITE_DONE)
			{
				printf("Full Name updated!! New NickName is %s\n", nickname);
			}
			sqlite3_finalize(stmt);
			break;
		case 3:
			printf("Input new email: ");
			fflush(stdin);
			fgets(email, 79, stdin);
			query = "update Client set email = ? where client_id = ?";
			sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
			sqlite3_bind_text(stmt, 1, email, strlen(email), SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, client_id);
			if (sqlite3_step(stmt) == SQLITE_DONE)
			{
				printf("Full Name updated!! New Email is %s\n", email);
			}
			sqlite3_finalize(stmt);
			break;
		default:
			break;
		}
	}
}
