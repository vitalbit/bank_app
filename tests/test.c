#include <check.h>
#include "../lib/sqlite3.h"
#include "../include/header.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

START_TEST(test_getAccountInfoById_create)
{
  char *zErrMsg = 0;
  char id[100] = "2";
  int rc = 0;
  sqlite3 *db;
  sqlite3_open("../Bank.sqlite", &db);

  rc = getAccountInfoById(db, zErrMsg, id);
  ck_assert_int_eq(0, 0);
}
END_TEST
 
Suite * getAccountInfoById_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_getAccountInfoById_create");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_getAccountInfoById_create);
  suite_add_tcase(s, tc_core);

  return s;
}

 int main(void)
 {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = getAccountInfoById_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }