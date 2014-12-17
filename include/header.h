void blockAccountByAccountID(sqlite3 *db, int account_id);

void unblockAccountByAccountID(sqlite3 *db, int account_id);

int isAccountBlock(sqlite3 *db, int account_id);

int credit(sqlite3 *db, int account_id, double credit_sum);

int getAccountInfoById(sqlite3 *db, char *errmsg, char *id);

void editClient(sqlite3 *db, int client_id, int field_num);

void addAccountToClient(sqlite3 *db, int client_id, int acc_type_id);

void getHistoryByAccountID(sqlite3 *db, int account_id);

int debitMoney(sqlite3 *db, double amount, char *operationDate, int clientID, int accountID);

int getUserInfo(sqlite3 *db, int clientID);

void deleteAccountByClientId(sqlite3 *db, int client_id);

int checkBlockOnClient(sqlite3 *db, int client_id);

