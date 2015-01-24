#include "common.h"
#include "peripherals.h"
#include "mathutil.h"
#include "quest.h"

 

uint8_t quest(vector v_B_c, vector v_sun_c, quaternion q_triad, uint8_t * p_w_ctrl)
{
  uint8_t w_ctrl = *p_w_ctrl;				
  static uint16_t time_since_light;			// initialise to 0 first
  static uint8_t light_prev = 1;
  uint16_t * sun_adc = (uint16_t *)Current_state.ss.reading;				// stores the value of the ADC board
  uint8_t light = 1, num_dark_sensors = 0, i, j;
  vector v_sun_m, v_B_m, v_cross_m, v_cross_c, v_mc_cross, v_mc_add;
  vector v_temp1, v_temp2;
  vector v_triad;
  float mu, nu, rho, k, triad;
  
  for(i = 0; i < N_SS; i++)
  {
    //Confirm SS_GAIN value and whether we have to use SS_MAX_ANGLE
    if(sun_adc[i] < (0.5 * SS_GAIN))			// sun_adc[]=voltage values of SS
      num_dark_sensors++;
  }
  if(num_dark_sensors == N_SS)
    light = 0;
  
  if(light)
  {
    if(!w_ctrl)
    {
      time_since_light += FRAME_TIME;
      if(time_since_light == 300)					// <=600 ??
        w_ctrl = 0;
    }
    
    if(light_prev == 0)
    {
      w_ctrl = 0;
      time_since_light = 0;
    }
    
    for(i = 0; i < (N_SS / 2); i++)
    {
      j = i * 2;
      if(sun_adc[j] > sun_adc[j + 1])					// ask shantanu
        v_sun_m[i] = (float)sun_adc[j];					// why is it converted to float
      else
        v_sun_m[i] = -1 * (float)sun_adc[j + 1];			// why is it multiplied by -1, understood , this is to remove the use of the ss_vector matrix
    }
    convert_unit_vector(v_sun_m);				// measured sun vector,b2
    
    v_B_m[0] = Current_state.mm.B_x;			// getting measured magnetic field vectors from the magnetometer
    v_B_m[1] = Current_state.mm.B_y;			// b1 
    v_B_m[2] = Current_state.mm.B_z;			// magnetic field is not normalized
    
    
    vector_cross_product(v_B_m, v_sun_m, v_cross_m);
    convert_unit_vector(v_cross_m);						// b3
    
    vector_cross_product(v_B_c, v_sun_c, v_cross_c);	// v_B_c=r1 , v_sun_c=r2
    convert_unit_vector(v_cross_c);						// r3
    
    mu = (1 + vector_dot_product(v_cross_m, v_cross_c)) * (MAG_WEIGHT * vector_dot_product(v_B_m, v_B_c) + (1 - MAG_WEIGHT) * vector_dot_product(v_sun_m, v_sun_c));
    
    vector_cross_product(v_B_m, v_B_c, v_temp1);		// b1 cross r1= v_temp1
    vector_cross_product(v_sun_m, v_sun_c, v_temp2);	// b2 cross r2= v_temp2
    for(i = 0; i < 3; i++)			
      v_temp2[i] = v_temp1[i] * MAG_WEIGHT + (1 - MAG_WEIGHT) * v_temp2[i];		
    vector_cross_product(v_cross_m, v_cross_c, v_mc_cross);			// b3 cross r3=v_mc_cross
    
    mu += vector_dot_product(v_mc_cross, v_temp2);				
    
    add_vectors(v_cross_m, v_cross_c, v_mc_add);		// b3+r3=v_mc_add
    
    nu = vector_dot_product(v_mc_add, v_temp2);			
    
    rho = sqrt(mu * mu + nu * nu);
    
    if(mu > 0)
    {
      k = 1 / (2 * sqrt(rho * (rho + mu) * (1 + vector_dot_product(v_cross_m, v_cross_c))));			// k= k_q_triad
      
      for(i = 0; i < 3; i++)
        v_triad[i] = v_mc_cross[i] * (rho + mu) + v_mc_add[i] * nu;										// v_triad= v_q_triad
      
      triad = (rho + mu) * (1 + vector_dot_product(v_cross_m, v_cross_c));								//triad= s_q_triad
      
    }
    else
    {
      k = 1 / (2 * sqrt(rho * (rho - mu) * (1 + vector_dot_product(v_cross_m, v_cross_c))));		// k_q_triad
      
      for(i = 0; i < 3; i++)
        v_triad[i] = v_mc_cross[i] * nu + v_mc_add[i] * (rho - mu);				// v_q_triad
      
      triad = nu * (1 + vector_dot_product(v_cross_m, v_cross_c));				// s_q_triad
      
    }
    for(i = 0; i < 3; i++)
      q_triad[i] = v_triad[i];
    q_triad[3] = triad;
      
    scalar_into_quaternion(q_triad, k);								// conversion into unit quaternion is not done
  }
  else
  {
    for(i = 0; i < 3; i++)
      q_triad[i] = 0;
    q_triad[3] = 1;										// sun vector is also not defined to be [1,0,0] in the else block
  }
  light_prev = light;
  return light;						// light might not be an integer
}

void omega_estimation(quaternion q_B, vector v_w)
{
  static quaternion q_B_old;
  quaternion dq, q;
  vector e, de, v_w_temp;
  static vector v_w_old = { 0.0, 0.0, 0.0 };
  matrix m_temp;
  float n;
  uint8_t i, j;
  
  for(i = 0; i < 4; i++)
  {
    dq[i] = (q_B[i] - q_B_old[i]) / FRAME_TIME;					// q_b_old is not initialised
    q[i] = (q_B[i] + q_B_old[i]) / 2;
  }
    
  for(i = 0; i < 3; i++)
  {
    de[i] = dq[i];
    e[i] = q[i];
  }
  n = q[3];

  matrix m_ex = { { 0, -2 * e[2], 2 * e[1] },				// different execution from that in matlab model
                  { 2 * e[2], 0, -2 * e[0] },
                  { -2 * e[1], 2 * e[0], 0 } };
  matrix m_I = { { 2 * n, 0, 0 },
                 { 0, 2 * n, 0 },
                 { 0, 0, 2 * n } };
  for(i = 0; i< 3; i++)
  {
    for(j = 0; j < 3; j++)
      m_temp[j][i] = m_I[i][j] - m_ex[i][j] + ((2 * e[i] * e[j]) / n);
  }
  
  vector_into_matrix(de, m_temp, v_w_temp);
  
  for(i = 0; i < 3; i++)
    v_w[i] = A_F * v_w_temp[i] + (1 - A_F) * v_w_old[i];
  
  copy_quaternion(q_B, q_B_old);
  copy_vector(v_w, v_w_old);
  
}
