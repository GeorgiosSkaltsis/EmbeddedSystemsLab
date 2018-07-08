/**
 *Filters Imlementation
 *Author: Dhaval Shah
 */
 
#include "in4073.h"
#include "filters.h"

#define prec 16

/**
 *Filters Initialisation
 *Author: Dhaval Shah
 */
void filter_init(void)
{
    p_bias      = 0;
    phi_kalman  = 0;
    sp          = 0;

	q_bias       = 0;
    theta_kalman = 0;
    sq           = 0;

	x0 = 0;
	x1 = 0;
	x2 = 0;
	y0 = 0;
	y1 = 0;
	y2 = 0;

    f_invC1p  = new_comp_fix(0,0b00000000010000000, prec);  //C1(roll) = 256
    f_invC1q  = new_comp_fix(0,0b00000000010000000, prec);  //C1(pitch) = 128
    f_invC2   = new_comp_fix(0,0b00000000000000100, prec);  //C2 = 10000
    f_p2phi   = new_comp_fix(0,0b00000000100000000, prec);  //P2PHI = 0.08
    f_q2theta = new_comp_fix(0,0b00000000100000000, prec);  //Q2THETA = 0.08

    f_p_kalman      = new_int_fix(p_kalman, prec);
    f_phi_kalman    = new_int_fix(phi_kalman, prec);
    f_phi_error     = new_int_fix(phi_error, prec);
    f_p_bias        = new_int_fix(p_bias, prec);
    f_phi_kalman0   = new_int_fix(phi_kalman0, prec);
    f_phi_error0    = new_int_fix(phi_error0, prec);
    f_phi_error2    = new_int_fix(phi_error2, prec);

    f_q_kalman      = new_int_fix(q_kalman, prec);
    f_theta_kalman  = new_int_fix(theta_kalman, prec);
    f_theta_error   = new_int_fix(theta_error, prec);
    f_q_bias        = new_int_fix(q_bias, prec);
    f_theta_kalman0 = new_int_fix(theta_kalman0, prec);
    f_theta_error0  = new_int_fix(theta_error0, prec);
    f_theta_error2  = new_int_fix(theta_error2, prec);
}

/**
 *Convert to fixed point sensor values
 *Author: Dhaval Shah
 */
void inttofix_kalman_roll()
{
    f_sp = new_int_fix(sp, prec);
    f_say = new_int_fix(say, prec);
}

/**
 *Convert to fixed point sensor values
 *Author: Dhaval Shah
 */
void inttofix_kalman_pitch()
{
    f_sq = new_int_fix(sq, prec);
    f_sax = new_int_fix(sax, prec);
}

/**
 *Kalman Filter for Roll
 *Author: Dhaval Shah
 */
void kalman_roll()
{
    f_p_kalman    = sub_fix(f_sp, f_p_bias);                //sp_prev - p_bias_prev 
    f_phi_kalman0 = mul_fix(f_p_kalman, f_p2phi);           //p_kalman*P2PHI
    f_phi_kalman  = add_fix(f_phi_kalman, f_phi_kalman0);   //phi_kalman_prev + phi_kalman0 
    f_phi_error   = sub_fix(f_phi_kalman, f_say);           // phi_kalman - say (corresponds to phi measurement) 
    f_phi_error0  = mul_fix(f_phi_error, f_invC1p);         // phi_error / C1
    f_phi_kalman  = sub_fix(f_phi_kalman, f_phi_error0);    //phi_kalman - phi_error0 
    f_phi_error2  = mul_fix(f_phi_error, f_invC2);
    f_p_bias      = add_fix(f_p_bias, f_phi_error2);

}

/**
 *Kalman Filter for Pitch
 *Author: Dhaval Shah
 */
void kalman_pitch()
{   
    f_q_kalman      = sub_fix(f_sq, f_q_bias);                  //sp_prev - p_bias_prev 
    f_theta_kalman0 = mul_fix(f_q_kalman, f_q2theta);           //p_kalman*P2PHI
    f_theta_kalman  = add_fix(f_theta_kalman, f_theta_kalman0); //phi_kalman_prev + phi_kalman0 
    f_theta_error   = sub_fix(f_theta_kalman, f_sax);           // phi_kalman - sax(corresponds to theta measurement) 
    f_theta_error0  = mul_fix(f_theta_error, f_invC1q);
    f_theta_kalman  = sub_fix(f_theta_kalman, f_theta_error0);  //phi_kalman - phi_error0 
    f_theta_error2  = mul_fix(f_theta_error, f_invC2);
    f_q_bias        = add_fix(f_q_bias, f_theta_error2);
   
}

/**
 *Convert to int sensor values
 *Author: Dhaval Shah
 */
void fixtoint_kalman_roll()
{
    sp  = int_fix(f_p_kalman);
    phi = int_fix(f_phi_kalman);
}

/**
 *Convert to int sensor values
 *Author: Dhaval Shah
 */
void fixtoint_kalman_pitch()
{
    sq    = int_fix(f_sq);
    theta = int_fix(f_theta_kalman);
}

/**
 * Butterworth filter for yaw.
 * 
 * Author: Georgios Skaltsis
 */
void filter_butter_yaw() 
{
y0 = (((sr << 8) + (sr << 6)) >> 14) + (((x1 << 9) + (x1 << 7)) >> 14) + (((x2 << 8) + (x2 << 6)) >> 14) + (((y1 << 14) + (y1 << 13)) >> 14) - (((y2 << 13) + (y2 << 11)) >> 14); // assume compiler optimizes ...
x2 = x1; // Adding more terms (with shifts) is done to improve the precision of the filter!
x1 = sr; 
y2 = y1; 
y1 = y0;
sr = y0;
}

/**
 * Filter all the newly received sensor data.
 * Author: Dhaval Shah
 */
void filter_data() 
{
    filter_kalman_roll();
    filter_kalman_pitch();
    filter_butter_yaw();
}
/**
 * Kalman Filter for roll.
 * Author: Dhaval Shah
 */
void filter_kalman_roll() 
{
	inttofix_kalman_roll();
	kalman_roll();
	fixtoint_kalman_roll();
}
/**
 * Kalman Filter for pitch.
 * Author: Dhaval Shah
 */
void filter_kalman_pitch() 
{
	inttofix_kalman_pitch();
	kalman_pitch();
	fixtoint_kalman_pitch();
}
