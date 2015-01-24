/**
 * @file  propagator.c
 * @brief Implements SGP Propagator model
*/

#include "common.h"
#include "mathutil.h"
#include "frame.h"
#include "peripherals.h"
#include "propagator.h"
#include "igrf.h"


static vector v_r;						
static vector v_v;          			

vector v_sat;               			// ??satellite vector??

extern volatile struct GPS_reading gps;

void copy_gps_reading(void)				
{
  static vector v_r_ecef, v_v_ecef;		// radial and velocity vector in ecef frame  
  
  Current_state.gps = gps;           	// 
  
  Current_state.gps.time_since_reading = 0;			// time since last reading 
  
  v_r_ecef[0] = (float)Current_state.gps.x / 1000;    // radial vector components calc
  v_r_ecef[1] = (float)Current_state.gps.y / 1000;
  v_r_ecef[2] = (float)Current_state.gps.z / 1000;
  
  v_v_ecef[0] = (float)Current_state.gps.v_x / 1000;  // velocity vector component calc
  v_v_ecef[1] = (float)Current_state.gps.v_y / 1000;
  v_v_ecef[2] = (float)Current_state.gps.v_z / 1000;
  
  get_seconds_since_equinox();   					 // time since equinox
  
  ecef2eci(v_r_ecef, v_r); 							// this is written in reverse order ;it should be(v_r,v_r_ecef) 
  ecef2eci(v_v_ecef, v_v); 							// "
  
  v_sat[0] = (((float)Current_state.gps.lat) / 10000000) * (M_PI / 180); 		// sat vector components calc, where is the 3rd vector ?
  v_sat[1] = (((float)Current_state.gps.lon) / 10000000) * (M_PI / 180);
  
  GPS_done = 0;										// flag indicating that GPS reading has been taken  and does -ve value indicate that reading has never been taken
}

void sgp_get_acceleration(vector v_g)    			// function for getting acceleration due to gravity
{
  vector v_r_ecef, v_g_ecef;                 		// radial and acceleration vector in ecef frame
  float R, R2, R3, R4;
  
  eci2ecef(v_r, v_r_ecef);							// conversion from eci to ecef frame, ye bhi ulta likha hai!!!!
  
  R = vector_norm(v_r_ecef);						// norm of position vector 
  R2 = pow(R, 2);							
  R2 = (1.5 * J2 * R_E2) / R2;					
  
  R3 = pow(R, 3);
  
  R4 = pow(R, 4);
  R4 = (7.5 * J2 * v_r_ecef[2] * R_E2) / R4;			
  
  v_g_ecef[0] = (-1 * GM * v_r_ecef[0] * (1 + R2 + R4)) / R3;			// gravity vector in ecef frame calc 
  v_g_ecef[1] = (-1 * GM * v_r_ecef[1] * (1 + R2 + R4)) / R3;
  v_g_ecef[2] = (-1 * GM * v_r_ecef[2] * (1 + 3 * R2 + R4)) / R3;
  
  ecef2eci(v_g_ecef, v_g);												// conversion from ecef to eci frame , it should be (v_g,v_g_ecef)
}

void sgp_orbit_propagator(void)						// orbit propagator model-ESTIMATED
{
  vector v_g;										// gravity vector
  vector v_v_old;									// old velocity vector
  float delta_t = ((float)FRAME_TIME) / 10;			// calc delta time
  uint8_t i, j;										
  
  for(i = 0; i < 10; i++)							// for loop for calculating radial and velocity vector
  {
    sgp_get_acceleration(v_g);
    
    copy_vector(v_v, v_v_old);
    
    for(j = 0; j < 3; j++)
      v_v[j] += v_g[j] * delta_t;    					 // v=at; calculation of the 3 components of the velocity vector
    
    for(j = 0; j < 3; j++)
      v_r[j] += ((v_v[j] + v_v_old[j]) / 2) * delta_t;   // s=v(avg) *delta t ;calculation of the radial vector components
  }

}

void sun_vector_estimator(vector v_sun_o)		// function for sun vector calculation in orbit frame  , I think this is estimated sun vector
{
  float time_in_days = (float)seconds_since_equinox / 86400;  		
  float lambda = (2 * M_PI * time_in_days) / 365.256363;
//  float EPSILON = 23.5 * M_PI / 180;					// the variable epsilon was not declared
  float delta, alpha;
  vector v_sun_eci;										// sun vector in eci frame
  
  delta = asin(sin(EPSILON) * sin(lambda));				
  alpha = atan2(cos(EPSILON) * sin(lambda), cos(lambda));				// defined differently in sun_calc.m!!!!
  
  v_sun_eci[0] = cos(delta) * cos(alpha);				// calc sun vector components in eci frame
  v_sun_eci[1] = cos(delta) * sin(alpha);
  v_sun_eci[2] = sin(delta);
  
  eci2orbit(v_r, v_v, v_sun_eci, v_sun_o);				// eci to orbit frame conversion 
}

void magnetic_field_estimator(vector v_B_o)		// fn for magnetic field estimator (this is unclear) ask a control member iska calc kahan hai ?
{
  float time_in_years = 2010 + (float)seconds_since_pivot / SECONDS_IN_YEAR;
  vector v_temp, v_r_lla, v_B_ned, v_B_eci;             // some unknown variable I need to know
  
  eci2ecef(v_r, v_temp);
  ecef2lla(v_temp, v_r_lla);
  ///* Save LLA vector for use in communications check routine
  copy_vector(v_r_lla, v_sat);
  
  igrf(v_r_lla, time_in_years, 8, v_B_ned);
  
  ned2ecef(v_B_ned, v_r_lla, v_temp);
  ecef2eci(v_temp, v_B_eci);
  eci2orbit(v_r, v_v, v_B_eci, v_B_o);
  
  scalar_into_vector(v_B_o, 1e-9);
  
}
