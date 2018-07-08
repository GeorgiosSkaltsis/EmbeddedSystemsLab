/*
 * Tests for the implementation of fixed-point arithmatic.
 * 
 * Author: Chiel Bruin
 */
 
#include <stdlib.h>
#include <stdbool.h>
#include <check.h>
#include "states.h"


START_TEST (test_1)
{
  current_mode = safe_mode;
  ck_assert_int_eq(true, transition_is_safe_to(panic_mode));
}
END_TEST

START_TEST (test_2)
{
  current_mode = panic_mode;
  ck_assert_int_eq(true, transition_is_safe_to(panic_mode));
}
END_TEST

START_TEST (test_3)
{
  current_mode = yaw_control_mode;
  ck_assert_int_eq(true, transition_is_safe_to(panic_mode));
}
END_TEST

START_TEST (test_4)
{
  current_mode = manual_mode;
  ck_assert_int_eq(true, transition_is_safe_to(panic_mode));
}
END_TEST

START_TEST (test_5)
{
  current_mode = manual_mode;
  ck_assert_int_eq(false, transition_is_safe_to(yaw_control_mode));
}
END_TEST

START_TEST (test_6)
{
  current_mode = yaw_control_mode;
  ck_assert_int_eq(false, transition_is_safe_to(manual_mode));
}
END_TEST


Suite * add_transitions_suite(void)
{
	Suite *s;
	TCase *tc_trans;

	/* Add test case */
	s = suite_create("State transitions");
	tc_trans = tcase_create("trns");
	 tcase_add_test(tc_trans, test_1);
	 tcase_add_test(tc_trans, test_2);
	 tcase_add_test(tc_trans, test_3);
	 tcase_add_test(tc_trans, test_4);
	 tcase_add_test(tc_trans, test_5);
	 tcase_add_test(tc_trans, test_6);
	suite_add_tcase(s, tc_trans);
	
	return s;
}
