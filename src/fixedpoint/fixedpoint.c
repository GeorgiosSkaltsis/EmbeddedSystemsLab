/*
 * Implementation of fixed-point arithmatic.
 * 
 * Author: Chiel Bruin
 */

#include "fixedpoint.h"

fix16_t new_fix(int32_t value, uint8_t precision)
{
	fix16_t result;
	 result.value = value;
	 result.precision = precision;
	return result;
}

fix16_t new_comp_fix(int32_t value, uint32_t fraction, uint8_t precision)
{
	fix16_t result;
	 result.value = (value << precision) | fraction;
	 result.precision = precision;
	return result;
}

fix16_t new_int_fix(int32_t value, uint8_t precision)
{
	fix16_t result;
	 result.value = value << precision;
	 result.precision = precision;
	return result;
}

int32_t int_fix(fix16_t fix) {
	return fix.value >> fix.precision;
}

int32_t floor_fix(fix16_t fix) {
	if (fix.value < 0) {
		int32_t res = fix.value >> fix.precision;
		//Check if it has fractional values 
		//(use bitmask as int32_t on 32-bit words has some extra leading bits)
		return ((fix.value << (16-fix.precision)) & INT16_MAX) > 0 ? --res : res;
	} else {
		return fix.value >> fix.precision;
	}
}

fix16_t add_fix(fix16_t l, fix16_t r) {
	return new_fix(l.value + r.value, l.precision);
}
fix16_t sub_fix(fix16_t l, fix16_t r) {
	return new_fix(l.value - r.value, l.precision);
}
fix16_t mul_fix(fix16_t l, fix16_t r) {
	int64_t res = l.value * r.value;  // We need extra space here
	return new_fix(res >> l.precision, l.precision);
}


fix16_t add_fix_checked(fix16_t l, fix16_t r) {
	return checked(add_fix, l, r);
}

fix16_t sub_fix_checked(fix16_t l, fix16_t r) {
	return checked(sub_fix, l, r);
}

fix16_t mul_fix_checked(fix16_t l, fix16_t r) {
	return checked(mul_fix, l, r);
}

fix16_t checked(fix16_t (*op) (fix16_t, fix16_t), fix16_t l, fix16_t r) {
	if (l.precision != r.precision)
	{
		int8_t delta = l.precision - r.precision;
		if (delta > 0) {
			l = new_fix(l.value >> delta, r.precision);
		} else {
			r = new_fix(r.value >> -delta, l.precision);
		}
	}
	return op(l, r);
}
