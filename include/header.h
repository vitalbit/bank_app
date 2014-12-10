void blockAccountByAccountID(sqlite3 *db, int account_id);

void unblockAccountByAccountID(sqlite3 *db, int account_id);

int isAccountBlock(sqlite3 *db, int account_id);

int isAccountBlock(sqlite3 *db, int account_id);

int credit(sqlite3 *db);

int getAccountInfoById(sqlite3 *db, char *errmsg);
