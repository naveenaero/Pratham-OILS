//
//  control.c
//  test-Detumbling
//
//  Created by Naveen Himthani on 29/11/14.
//
//


#include "common.h"
#include "control.h"
#include "mathutil.h"
#include "peripherals.h"


static vector v_B;
static vector v_w = { 0.0, 0.0, 0.0 };
static quaternion q_o;
static uint8_t first_B = 1, light, w_ctrl = 1;

void detumbling(vector v_m_D)
    {
        static vector v_B_old;
        
        /// Check if it is the first magnetic field reading
        if(first_B)
        {
            
            /// copy the current magnetic field to the old magnetic field reading
            copy_vector(v_B, v_B_old);
            
            /// declare the flag of first magnetic field reading to 0
            first_B = 0;
            return;
        }
        
        vector v_B_avg, v_dB;
        float factor;
        uint8_t i;
        
        /// loop for calculating Bdot and average Magnetic field
        for(i = 0; i < 3; i++)
        {
            v_dB[i] = (v_B[i] - v_B_old[i]) / FRAME_TIME;
            v_B_avg[i] = (v_B[i] + v_B_old[i]) / 2;
        }
        
        /// calculating factor=(-1* K_DETUMBLING * MAG_B)/ (norm of average magnetic field)
        factor = -1 / vector_norm(v_B_avg);
        
        /// loop for calculating moment by multiplying factor and Bdot
        for(i = 0; i < 3; i++)
        {
            v_m_D[i] = factor *v_dB[i];
            
        }
        
        /// subsequent copying of current to old magnetic field
        copy_vector(v_B, v_B_old);
        
    }


void apply_torque(vector v_m_D)
    {
    uint8_t i;
        
    /// loop for moment saturation
    for(i = 0; i < 3; i++)
    {
        if(fabs(v_m_D[i]) > M_MAX)
        {
            if (fabs(v_m_D[i])/v_m_D[i] == -1)
                v_m_D[i] = -1*M_MAX;
            else
                v_m_D[i] = M_MAX;
        }
        
    }
        
    
    
    /// Multiplication of moment by 1/(No. of turns * Area) of the torquer
    scalar_into_vector(v_m_D, 1.0 / (N_TURNS * AREA));
        
        
    /// Current saturation
    for(i = 0; i < 3; i++)
    {
        if(fabs(v_m_D[i]) > I_MAX)
        {
            if (fabs(v_m_D[i])/v_m_D[i] == -1)
                v_m_D[i] = -1*I_MAX;
            else
                v_m_D[i] = I_MAX;
        }
    }
    
    vg[0] = fabs((v_m_D[0] * PWM_RES) / I_MAX);
    vg[1] = fabs((v_m_D[1] * PWM_RES) / I_MAX);
    vg[2] = fabs((v_m_D[2] * PWM_RES) / I_MAX);
    
    /// Assigning required values of currents to be given to torquers
    Current_state.pwm.x = fabs((v_m_D[0] * PWM_RES) / I_MAX);
    Current_state.pwm.y = fabs((v_m_D[1] * PWM_RES) / I_MAX);
    Current_state.pwm.z = fabs((v_m_D[2] * PWM_RES) / I_MAX);
    
    /// Assigning direction of current in the torquers
    if(v_m_D[0] < 0)
        Current_state.pwm.x_dir = 1;
    if(v_m_D[1] < 0)
        Current_state.pwm.y_dir = 1;
    if(v_m_D[2] < 0)
        Current_state.pwm.z_dir = 1;
    
    set_PWM();
}

void control(void)
{
    vector v_m_D;
    
    
    /// typecasting to float is necessary because int/int will be int and not float.
    v_B[0] = ((float)Bx)/15000;
    v_B[1] = ((float)By)/15000;
    v_B[2] = ((float)Bz)/15000;
    
    
    
    detumbling(v_m_D);
    
    /// Check if the satellite is in detumbling mode
    if (vector_norm(v_m_D) > -0)
        {
            apply_torque(v_m_D);
        }
//    else
//        reset_PWM();
    
}


        


