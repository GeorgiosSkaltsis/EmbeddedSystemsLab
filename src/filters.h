/**
 *Header file for Filters
 *
 *Author: Dhaval Shah
 *
 *MAY 2018
 */

#ifndef FILTERS_H__
#define FILTERS_H__

#include "fixedpoint.h"

int32_t  P2PHI; 
int32_t  Q2THETA; 
int32_t  C1;
int32_t  C2;
int32_t  invC1;
int32_t  invC2;
int32_t  p_kalman, phi_kalman, phi_error, p_bias, phi_kalman0, phi_error0, phi_error2;
int32_t  q_kalman, theta_kalman, theta_error, q_bias, theta_kalman0, theta_error0, theta_error2;
int32_t  x0, x1, x2, y0, y1, y2;
fix16_t  f_C1, f_C2, f_invC1p, f_invC1q, f_invC2 ;
fix16_t  f_p_kalman, f_phi_kalman, f_phi_error, f_p_bias, f_phi_kalman0, f_phi_error0, f_phi_error2, 
         f_p2phi, f_sp, f_sax;
fix16_t  f_q_kalman, f_theta_kalman, f_theta_error, f_q_bias, f_theta_kalman0, f_theta_error0, f_theta_error2, 
         f_q2theta, f_sq, f_say;


void inttofix_kalman_roll();
void inttofix_kalman_pitch();
void kalman_roll();
void kalman_pitch();
void fixtoint_kalman_roll();
void fixtoint_kalman_pitch();
void filter_kalman_roll();
void filter_kalman_pitch();
void filter_butter_yaw();
void filter_init();
void filter_data();

#endif