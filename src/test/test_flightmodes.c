#include <stdlib.h>
#include <check.h>
#include "states.h"
#include "in4073.h"

void setup()
{
	ae[0] = 0;
	ae[1] = 0;
	ae[2] = 0;
	ae[3] = 0;
}

START_TEST (test_yaw1)
{
	mode_step_yaw_control(0, 0, 0, 0);

	ck_assert_int_eq(ae[0], 0);
	ck_assert_int_eq(ae[1], 0);
	ck_assert_int_eq(ae[2], 0);
	ck_assert_int_eq(ae[3], 0);
}
END_TEST

START_TEST (test_calibration1)
{
	ae[0] = 100;
	mode_step_calibration(0, 0, 0, 0);

	ck_assert_int_eq(ae[0], 0);
	ck_assert_int_eq(ae[1], 0);
	ck_assert_int_eq(ae[2], 0);
	ck_assert_int_eq(ae[3], 0);
}
END_TEST

START_TEST (test_calibration2)
{
	ae[0] = 100;
	mode_step_calibration(10, 10, 10, 10);

	ck_assert_int_eq(ae[0], 0);
	ck_assert_int_eq(ae[1], 0);
	ck_assert_int_eq(ae[2], 0);
	ck_assert_int_eq(ae[3], 0);
}
END_TEST




Suite * add_flightmodes_suite(void)
{
	Suite *s;
	
	/* Yaw test case */
	TCase *tc_yaw;
	s = suite_create("Yaw mode");
	tc_yaw = tcase_create("yaw");
	 tcase_add_test(tc_yaw, test_yaw1);
	suite_add_tcase(s, tc_yaw);
	
	
	/* Calibration test case */
	TCase *tc_cal;
	s = suite_create("Calibration mode");
	tc_cal = tcase_create("calibration");
	 tcase_add_test(tc_cal, test_calibration1);
	 tcase_add_test(tc_cal, test_calibration2);
	suite_add_tcase(s, tc_cal);

	return s;
}
