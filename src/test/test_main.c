/*
 * Main file for the testing framework.
 * Makes use of Check (https://libcheck.github.io/check/)
 * as a testing framework.
 * 
 * This file adds all the test suites to the runnner.
 * 
 * Author: Chiel Bruin
 */

#include "test_main.h"

// Just mock this method
int check_timer_flag() {
	return 0;
}

int run(Suite* (*constructor) (void))
{
	Suite *s;
	SRunner *sr;

	s = constructor();
	sr = srunner_create(s);
	
	srunner_run_all(sr, CK_NORMAL);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return number_failed;
}

int main(int argc, char **argv)
{
	int number_failed = 0;
	
	number_failed += run(add_fixedpoint_suite);
	number_failed += run(add_pc_terminal_suite);
	number_failed += run(add_flightmodes_suite);
	number_failed += run(add_transitions_suite);
	
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
