/*
 * Headers for the implementation of fixed-point arithmatic.
 * 
 * Author: Chiel Bruin
 */
 
#ifndef FIXEDPOINT_H__
#define FIXEDPOINT_H__

#include <inttypes.h>

typedef struct fix16_t {
    int32_t value;
    uint8_t precision;
} fix16_t;

/**
 * Create a new fixed-point number with given value and precision.
 */
fix16_t new_fix(int32_t value, uint8_t precision);

/**
 * Create a new fixed-point number with given integer value and precision.
 */
fix16_t new_int_fix(int32_t value, uint8_t precision);

/**
 * Create a new fixed-point number with given integer value, fraction and precision.
 */
fix16_t new_comp_fix(int32_t value, uint32_t fraction, uint8_t precision);

/**
 * Adds two fixed point values.
 * Does not check precision.
 */
fix16_t add_fix(fix16_t l, fix16_t r);

/**
 * Subtract two fixed point values.
 * Does not check precision.
 */
fix16_t sub_fix(fix16_t l, fix16_t r);

/**
 * Multiplies two fixed point values.
 * Does not check precision.
 */
fix16_t mul_fix(fix16_t l, fix16_t r);

/**
 * Checked versions of the operators.
 * Makes sure that the used precisions are compatible.
 * When not the lowest precision is kept.
 */
fix16_t add_fix_checked(fix16_t l, fix16_t r);
fix16_t sub_fix_checked(fix16_t l, fix16_t r);
fix16_t mul_fix_checked(fix16_t l, fix16_t r);

/**
 * Makes sure that the used precisions are compatible.
 * When not the lowest precision is kept.
 * Applies the checked values to the given function.
 */
fix16_t checked(fix16_t (*op) (fix16_t, fix16_t), fix16_t l, fix16_t r);

/**
 * Get the floored value of the fixed-point number.
 */
int32_t floor_fix(fix16_t fix);

/**
 * Get the integer component of the fixed-point number.
 */
int32_t int_fix(fix16_t fix);

#endif
