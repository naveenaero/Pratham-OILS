/**
 *@file  controller.h
 *@brief Contains the functions for the Control Law
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

/// Magnetic field constant
#define MAG_B 2.5e-5

/// Detumbling constant
#define K_DETUMBLING 4e4

/// Tolerance value of W for Detumbling to Nominal mode transfer
#define TOLW_D2N  4e-3

/// Tolerance value of W for Nominal to Detumbling mode transfer
#define TOLW_N2D  10e-3

/// Time period for compulsory Detumbling
#define D_TIME 2000

/// Checktime for Nominal to Detumbling Mode transfer
#define N2D_TIME 10000

/// Saturation Moment
#define M_MAX 0.95

/// Number of turns in the torquer
#define N_TURNS 60

/// Cross-sectional area of the torquer
#define AREA 0.0442483613596446

/// PWM resolution of the torquer
#define PWM_RES 0xFFFF

/// Saturation current
#define I_MAX 1 	// 0.5 in simulink

#define DETUMBLING 0
#define NOMINAL 1


#include "mathutil.h"


void control(void);
void detumbling(vector v_m_D);
void apply_torque(vector v_m);

#endif
