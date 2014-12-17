#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sqlite3.h"

#include "header.h"

void deleteClientByClientID(sqlite3 *db, int client_id){
	sqlite3_stmt *stmt = NULL;
	char *sqlDeleteClientByClientID = "UPDATE Client SET is_delete = 1 WHERE client_id = ?";
	sqlite3_prepare_v2(db, sqlDeleteClientByClientID, strlen(sqlDeleteClientByClientID), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, client_id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void restoreClientByClientID(sqlite3 *db, int client_id){
	sqlite3_stmt *stmt = NULL;
	char *sqlRestoreClientByClientID = "UPDATE Client SET is_delete = 0 WHERE client_id = ?";
	sqlite3_prepare_v2(db, sqlRestoreClientByClientID, strlen(sqlRestoreClientByClientID), &stmt, NULL);
	sqlite3_bind_int(stmt, 1, client_id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}
