/*
 * Tests for the implementation of fixed-point arithmatic.
 * 
 * Author: Chiel Bruin
 */
 
#include <stdlib.h>
#include <check.h>
#include "fixedpoint.h"

/*
 * ADDITION TESTS
 */
 
START_TEST (test_simple_add1)
{
  fix16_t a = new_fix(0b0111111111100000, 5); // 1023
  fix16_t b = new_fix(0b0000000000000000, 5); // 0
  
  fix16_t result = add_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0111111111100000); // 1023
}
END_TEST

START_TEST (test_simple_add2)
{
  fix16_t a = new_fix(0b0010101010100000, 5); // 341
  fix16_t b = new_fix(0b0101010101000000, 5); // 682
  
  fix16_t result = add_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0111111111100000); // 1023
}
END_TEST

START_TEST (test_simple_add3)
{
  fix16_t a = new_fix(0b0000000000011010, 5); // 0.5 + .25 + .0625 = .8125
  fix16_t b = new_fix(0b0000000000000001, 5); // 0.03125
  
  fix16_t result = add_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000000011011); // 0.5 + .25 + .0625 + 0.03125 = 0.84275
}
END_TEST

START_TEST (test_add_diff_prec1)
{
  fix16_t a = new_int_fix(1, 10); // 1
  fix16_t b = new_int_fix(1, 5);  // 1
  
  fix16_t result = checked(add_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000001000000); // 2
}
END_TEST

START_TEST (test_add_diff_prec2)
{
  fix16_t a = new_int_fix(3, 5);  // 3
  fix16_t b = new_int_fix(3, 10); // 3
  
  fix16_t result = checked(add_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000011000000); // 6
}
END_TEST

START_TEST (test_add_diff_prec3)
{
  fix16_t a = new_fix(0b0000000001110000, 5);  // 3.5
  fix16_t b = new_fix(0b0000111000000000, 10); // 3.5
  
  fix16_t result = checked(add_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000011100000); // 7
}
END_TEST


/*
 * SUBTRACTION TESTS
 */
 
START_TEST (test_simple_sub1)
{
  fix16_t a = new_fix(0b0111111111100000, 5); // 1023
  fix16_t b = new_fix(0b0000000000000000, 5); // 0
  
  fix16_t result = sub_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0111111111100000); // 1023
}
END_TEST

START_TEST (test_simple_sub2)
{
  fix16_t a = new_int_fix(6, 5); // 6
  fix16_t b = new_int_fix(7, 5); // 7
  
  fix16_t result = sub_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b1111111111100000); // -1
}
END_TEST

START_TEST (test_simple_sub3)
{
  fix16_t a = new_fix(0b0000000000010000, 5); // 0.5
  fix16_t b = new_fix(0b0000000000001000, 5); // 0.25
  
  fix16_t result = sub_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000000001000); // .25
}
END_TEST

START_TEST (test_simple_sub4)
{
  fix16_t a = new_int_fix(0, 5); // 0
  fix16_t b = new_int_fix(1023, 5); // 1023
  
  fix16_t result = sub_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b1000000000100000); // -1023
}
END_TEST

START_TEST (test_sub_diff_prec1)
{
  fix16_t a = new_int_fix(2, 10); // 2
  fix16_t b = new_int_fix(1, 5);  // 1
  
  fix16_t result = checked(sub_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000000100000); // 1
}
END_TEST

START_TEST (test_sub_diff_prec2)
{
  fix16_t a = new_int_fix(3, 10);  // 3
  fix16_t b = new_int_fix(3, 5); // 3
  
  fix16_t result = checked(sub_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000000000000); // 0
}
END_TEST

START_TEST (test_sub_diff_prec3)
{
  fix16_t a = new_fix(0b0000000010010000, 5);  // 4.5
  fix16_t b = new_fix(0b0000111000000000, 10); // 3.5
  
  fix16_t result = checked(sub_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000000100000);
  ck_assert_int_eq(floor_fix(result), 1);
}
END_TEST


/*
 * MULTIPLICATION TESTS
 */
 
START_TEST (test_mul_simple1)
{
  fix16_t a = new_int_fix(5, 5); // 5
  fix16_t b = new_int_fix(2, 5); // 2
  
  fix16_t result = mul_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000101000000);
  ck_assert_int_eq(floor_fix(result), 10);
}
END_TEST

START_TEST (test_mul_simple2)
{
  fix16_t a = new_comp_fix(5, 0b10000, 5); // 5.5
  fix16_t b = new_int_fix(2, 5);           // 2
  
  fix16_t result = mul_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000101100000);
  ck_assert_int_eq(floor_fix(result), 11);
}
END_TEST


START_TEST (test_mul_simple3)
{
  fix16_t a = new_comp_fix(5, 0b10000, 5); // 5.5
  fix16_t b = new_int_fix(-2, 5);          // -2
  
  fix16_t result = mul_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b1111111010100000);
  ck_assert_int_eq(floor_fix(result), -11);
}
END_TEST

START_TEST (test_mul_simple4)
{
  fix16_t a = new_comp_fix(5, 0b10000, 5); // 5.5
  fix16_t b = new_comp_fix(2, 0b10000, 5); // 2.5
  
  fix16_t result = mul_fix(a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000110111000);
  ck_assert_int_eq(floor_fix(result), 13);
}
END_TEST

START_TEST (test_mul_simple5)
{
  fix16_t a = new_comp_fix(500, 0b00000, 4); // 500
  fix16_t b = new_comp_fix(4, 0b00000, 4);   // 4
  
  fix16_t result = mul_fix(a, b);
  
  ck_assert_int_eq(result.precision, 4);
  ck_assert_int_eq(floor_fix(result), 2000);
}
END_TEST

START_TEST (test_mul_diff_prec1)
{
  fix16_t a = new_int_fix(2, 10);          // 2
  fix16_t b = new_comp_fix(5, 0b10000, 5); // 5.5
  
  fix16_t result = checked(mul_fix, a, b);
  
  ck_assert_int_eq(result.precision, 5);
  ck_assert_int_eq(result.value, (int16_t) 0b0000000101100000);
  ck_assert_int_eq(floor_fix(result), 11);
}
END_TEST


/*
 * COMMON TESTS
 */
 
START_TEST (test_common_floor1)
{
  fix16_t a = new_comp_fix(2, 0b10011, 10);
  ck_assert_int_eq(floor_fix(a), 2);
}
END_TEST

START_TEST (test_common_floor2)
{
  fix16_t a = new_int_fix(2, 10);
  ck_assert_int_eq(floor_fix(a), 2);
}
END_TEST

START_TEST (test_common_floor3)
{
  fix16_t a = new_comp_fix(-2, 0b10000, 10);
  ck_assert_int_eq(floor_fix(a), -3);
}
END_TEST

START_TEST (test_common_floor4)
{
  fix16_t a = new_comp_fix(-2, 0b01000, 10);
  ck_assert_int_eq(floor_fix(a), -3);
}
END_TEST

START_TEST (test_common_floor5)
{
  fix16_t a = new_comp_fix(-2, 0b00000, 10);
  ck_assert_int_eq(floor_fix(a), -2);
}
END_TEST

START_TEST (test_common_floor6)
{
  fix16_t a = new_comp_fix(-2, 0b00001, 10);
  ck_assert_int_eq(floor_fix(a), -3);
}
END_TEST

START_TEST (test_common_int1)
{
  fix16_t a = new_comp_fix(2, 0b10011, 10);
  ck_assert_int_eq(int_fix(a), 2);
}
END_TEST

START_TEST (test_common_int2)
{
  fix16_t a = new_int_fix(2, 10);
  ck_assert_int_eq(int_fix(a), 2);
}
END_TEST

START_TEST (test_common_int3)
{
  fix16_t a = new_comp_fix(-2, 0b10010, 10);
  ck_assert_int_eq(int_fix(a), -2);
}
END_TEST

START_TEST (test_common_int4)
{
  fix16_t a = new_comp_fix(-2, 0b00000, 10);
  ck_assert_int_eq(int_fix(a), -2);
}
END_TEST





Suite * add_fixedpoint_suite(void)
{
	Suite *s;
	TCase *tc_add;

	/* Add test case */
	s = suite_create("Fixedpoint");
	tc_add = tcase_create("add");
	 tcase_add_test(tc_add, test_simple_add1);
	 tcase_add_test(tc_add, test_simple_add2);
	 tcase_add_test(tc_add, test_simple_add3);
	 tcase_add_test(tc_add, test_add_diff_prec1);
	 tcase_add_test(tc_add, test_add_diff_prec2);
	 tcase_add_test(tc_add, test_add_diff_prec3);
	suite_add_tcase(s, tc_add);
	
	
	/* Sub test case */
	TCase *tc_sub;
	tc_sub = tcase_create("sub");
	 tcase_add_test(tc_sub, test_simple_sub1);
	 tcase_add_test(tc_sub, test_simple_sub2);
	 tcase_add_test(tc_sub, test_simple_sub3);
	 tcase_add_test(tc_sub, test_simple_sub4);
	 tcase_add_test(tc_sub, test_sub_diff_prec1);
	 tcase_add_test(tc_sub, test_sub_diff_prec2);
	 tcase_add_test(tc_sub, test_sub_diff_prec3);
	suite_add_tcase(s, tc_sub);


	/* Mul test case */
	TCase *tc_mul;
	tc_mul = tcase_create("mul");
	 tcase_add_test(tc_mul, test_mul_simple1);
	 tcase_add_test(tc_mul, test_mul_simple2);
	 tcase_add_test(tc_mul, test_mul_simple3);
	 tcase_add_test(tc_mul, test_mul_simple4);
	 tcase_add_test(tc_mul, test_mul_simple5);
	 tcase_add_test(tc_mul, test_mul_diff_prec1);
	suite_add_tcase(s, tc_mul);
	
	/* Init test case */
	TCase *tc_common;
	tc_common = tcase_create("common");
	 tcase_add_test(tc_common, test_common_floor1);
	 tcase_add_test(tc_common, test_common_floor2);
	 tcase_add_test(tc_common, test_common_floor3);
	 tcase_add_test(tc_common, test_common_floor4);
	 tcase_add_test(tc_common, test_common_floor5);
	 tcase_add_test(tc_common, test_common_floor6);
	 tcase_add_test(tc_common, test_common_int1);
	 tcase_add_test(tc_common, test_common_int2);
	 tcase_add_test(tc_common, test_common_int3);
	 tcase_add_test(tc_common, test_common_int4);
	suite_add_tcase(s, tc_common);
	
	return s;
}
