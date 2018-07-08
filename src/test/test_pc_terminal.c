/*
 * Tests for the implementation of the PC-terminal.
 * 
 * Author: Chiel Bruin
 */
 
#include <stdlib.h>
#include <check.h>
#include "util.h"
	
START_TEST (test_offset1)
{
  int16_t val = 0;
  int16_t offset = 0;
  ck_assert_int_eq(safe_offset(val, offset), 0);
}
END_TEST

START_TEST (test_offset2)
{
  int16_t val = 0;
  int16_t offset = 10;
  ck_assert_int_eq(safe_offset(val, offset), 10);
}
END_TEST

START_TEST (test_offset3)
{
  int16_t val = 0;
  int16_t offset = -10;
  ck_assert_int_eq(safe_offset(val, offset), -10);
}
END_TEST

START_TEST (test_offset4)
{
  int16_t val = INT16_MAX;
  int16_t offset = 0;
  ck_assert_int_eq(safe_offset(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_offset5)
{
  int16_t val = INT16_MAX;
  int16_t offset = 10;
  ck_assert_int_eq(safe_offset(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_offset6)
{
  int16_t val = INT16_MAX-10;
  int16_t offset = 20;
  ck_assert_int_eq(safe_offset(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_offset7)
{
  int16_t val = INT16_MAX;
  int16_t offset = -10;
  ck_assert_int_eq(safe_offset(val, offset), 32757);
}
END_TEST

START_TEST (test_offset8)
{
  int16_t val = INT16_MIN;
  int16_t offset = 0;
  ck_assert_int_eq(safe_offset(val, offset), INT16_MIN);
}
END_TEST

START_TEST (test_offset9)
{
  int16_t val = INT16_MIN;
  int16_t offset = -10;
  ck_assert_int_eq(safe_offset(val, offset), INT16_MIN);
}
END_TEST

START_TEST (test_offset10)
{
  int16_t val = INT16_MIN;
  int16_t offset = 10;
  ck_assert_int_eq(safe_offset(val, offset), -32758);
}
END_TEST

START_TEST (test_offset11)
{
  int16_t val = INT16_MIN+10;
  int16_t offset = -20;
  ck_assert_int_eq(safe_offset(val, offset), INT16_MIN);
}
END_TEST




START_TEST (test_lift0)
{
  int16_t val = 0;
  int16_t offset = 0;
  ck_assert_int_eq((int) calculate_lift(val, offset), 16384);
}
END_TEST

START_TEST (test_lift1)
{
  int16_t val = INT16_MIN;
  int16_t offset = 0;
  ck_assert_int_eq((int) calculate_lift(val, offset), 0);
}
END_TEST

START_TEST (test_lift2)
{
  int16_t val = INT16_MIN;
  int16_t offset = 10;
  ck_assert_int_eq((int) calculate_lift(val, offset), 10);
}
END_TEST

START_TEST (test_lift3)
{
  int16_t val = INT16_MIN;
  int16_t offset = -10;
  ck_assert_int_eq((int) calculate_lift(val, offset), 0);
}
END_TEST

START_TEST (test_lift4)
{
  int16_t val = INT16_MIN;
  int16_t offset = INT16_MIN;
  ck_assert_int_eq((int) calculate_lift(val, offset), 0);
}
END_TEST

START_TEST (test_lift5)
{
  int16_t val = INT16_MIN;
  int16_t offset = INT16_MAX;
  ck_assert_int_eq((int) calculate_lift(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_lift6)
{
  int16_t val = INT16_MAX;
  int16_t offset = 0;
  ck_assert_int_eq((int) calculate_lift(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_lift7)
{
  int16_t val = INT16_MAX;
  int16_t offset = 10;
  ck_assert_int_eq((int) calculate_lift(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_lift8)
{
  int16_t val = INT16_MAX;
  int16_t offset = -10;
  ck_assert_int_eq((int) calculate_lift(val, offset), 32757);
}
END_TEST

START_TEST (test_lift9)
{
  int16_t val = INT16_MAX;
  int16_t offset = INT16_MAX;
  ck_assert_int_eq((int) calculate_lift(val, offset), INT16_MAX);
}
END_TEST

START_TEST (test_lift10)
{
  int16_t val = INT16_MAX;
  int16_t offset = INT16_MIN;
  ck_assert_int_eq((int) calculate_lift(val, offset), 0);
}
END_TEST

START_TEST (test_lift11)
{
  int16_t val = 0;
  int16_t offset = INT16_MIN;
  ck_assert_int_eq((int) calculate_lift(val, offset), 0);
}
END_TEST

START_TEST (test_lift12)
{
  int16_t val = 0;
  int16_t offset = INT16_MAX;
  ck_assert_int_eq((int) calculate_lift(val, offset), INT16_MAX);
}
END_TEST


Suite * add_pc_terminal_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("PC Terminal");
	tc = tcase_create("Offset");
	 tcase_add_test(tc, test_offset1);
	 tcase_add_test(tc, test_offset2);
	 tcase_add_test(tc, test_offset3);
	 tcase_add_test(tc, test_offset4);
	 tcase_add_test(tc, test_offset5);
	 tcase_add_test(tc, test_offset6);
	 tcase_add_test(tc, test_offset7);
	 tcase_add_test(tc, test_offset8);
	 tcase_add_test(tc, test_offset9);
	 tcase_add_test(tc, test_offset10);
	 tcase_add_test(tc, test_offset11);
	suite_add_tcase(s, tc);
	
	tc = tcase_create("Lift");
	 tcase_add_test(tc, test_lift0);
	 tcase_add_test(tc, test_lift1);
	 tcase_add_test(tc, test_lift2);
	 tcase_add_test(tc, test_lift3);
	 tcase_add_test(tc, test_lift4);
	 tcase_add_test(tc, test_lift5);
	 tcase_add_test(tc, test_lift6);
	 tcase_add_test(tc, test_lift7);
	 tcase_add_test(tc, test_lift8);
	 tcase_add_test(tc, test_lift9);
	 tcase_add_test(tc, test_lift10);
	 tcase_add_test(tc, test_lift11);
	 tcase_add_test(tc, test_lift12);
	suite_add_tcase(s, tc);
	
	return s;
}

