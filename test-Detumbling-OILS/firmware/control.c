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
//#include "propagator.h"
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
        factor = (-1 * K_DETUMBLING * MAG_B) / vector_norm(v_B_avg);
        
        /// loop for calculating moment by multiplying factor and Bdot
        for(i = 0; i < 3; i++)
            v_m_D[i] = factor * v_dB[i];
        
        /// subsequent copying of current to old magnetic field
        copy_vector(v_B, v_B_old);
    }


void apply_torque(vector v_m)
    {
    uint8_t i;
    
    /// loop for moment saturation
    for(i = 0; i < 3; i++)
    {
        if(fabs(v_m[i]) > M_MAX)
        {
            if (abs(v_m[i])/v_m[i] == -1)
                v_m[i] = -1*M_MAX;
            else
                v_m[i] = M_MAX;
        }
        
    }
    
    /// Multiplication of moment by 1/(No. of turns * Area) of the torquer
    scalar_into_vector(v_m, 1.0 / (N_TURNS * AREA));
    
    /// Current saturation
    for(i = 0; i < 3; i++)
    {
        if(fabs(v_m[i]) > I_MAX)
        {
            if (abs(v_m[i])/v_m[i] == -1)
                v_m[i] = -1*I_MAX;
            else
                v_m[i] = I_MAX;
        }

        
    }
    
    /// Assigning required values of currents to be given to torquers
    Current_state.pwm.x = fabs((v_m[0] * PWM_RES) / I_MAX);
    Current_state.pwm.y = fabs((v_m[1] * PWM_RES) / I_MAX);
    Current_state.pwm.z = fabs((v_m[2] * PWM_RES) / I_MAX);
    
    /// Assigning direction of current in the torquers
    if(v_m[0] < 0)
        Current_state.pwm.x_dir = 1;
    if(v_m[1] < 0)
        Current_state.pwm.y_dir = 1;
    if(v_m[2] < 0)
        Current_state.pwm.z_dir = 1;
}

void control(void)
{
    vector v_m_D, v_B;
    
    /// Reset PWM so that there is no interference in taking magmeter readings
//    reset_PWM();
    
    /// Give delay of 100 microseconds
//    _delay_us(100);
//    
    /// Read Magnetometer
//    read_MM();
    
    
    
    
    ///Set the torquer values calculated in the last frame
    set_PWM();
    
    
    v_B[0] = Current_state.mm.B_x;
    v_B[1] = Current_state.mm.B_y;
    v_B[2] = Current_state.mm.B_z;
    
//    if (v_B[0] != 0 || v_B[1] != 0 || v_B[2] != 0)
//    {
//        /// Calculate Detumbling mode moment
        detumbling(v_m_D);
        
        /// Check if the satellite is in detumbling mode
        if (vector_norm(v_m_D) > 2)
        {
            apply_torque(v_m_D);
        }
    else
        reset_PWM();

//    }
    
//    else
//    {
//        /// Apply no control/torque when all the magnetic field readings are zero
//        reset_PWM();
//        
//    }
    
}


        


