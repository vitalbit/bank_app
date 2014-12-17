#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

void checkBlockOnClient(sqlite3 *db, int client_id)
{
	char *query = "select is_block from Client where client_id=?;";
	sqlite3_stmt *stmt = NULL;
	sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, client_id);
	int result = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		result = atoi((char*)sqlite3_column_text(stmt, 0));
	}

	if (result == 0)
	{
		printf("Client is not blocked\n");
	}
	else
	{
		printf("Client is blocked\n");
	}
}

