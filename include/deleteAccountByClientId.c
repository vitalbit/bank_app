#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

void deleteAccountByClientId(sqlite3 *db, int client_id)
{
	char *query = "delete from Account where client_id=?;";
	sqlite3_stmt *stmt = NULL;
	sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, client_id);
	if (sqlite3_step(stmt) == SQLITE_DONE)
	{
		printf("Account has been deleted.\n");
	}
	sqlite3_finalize(stmt);
}

