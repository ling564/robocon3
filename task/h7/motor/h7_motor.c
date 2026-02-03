#include "h7_motor.h"

int float_to_uint(float x_float, float x_min, float x_max, int bits)
{
    /* Converts a float to an unsigned int, given range and number of bits */
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x_float-offset)*((float)((1<<bits)-1))/span);
}

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
    /* converts unsigned int to float, given range and number of bits */
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

void dm_motor_fbdata(motor_t *motor, uint8_t *rx_data)
{
    motor->id = (rx_data[0])&0x0F;
    motor->state = (rx_data[0])>>4;
    motor->p_int=(rx_data[1]<<8)|rx_data[2];
    motor->v_int=(rx_data[3]<<4)|(rx_data[4]>>4);
    motor->t_int=((rx_data[4]&0xF)<<8)|rx_data[5];
    motor->pos = uint_to_float(motor->p_int, -motor->PMAX, motor->PMAX, 16); 
    motor->vel = uint_to_float(motor->v_int, -motor->VMAX, motor->VMAX, 12); 
    motor->tor = uint_to_float(motor->t_int, -motor->TMAX, motor->TMAX, 12); 
    motor->Tmos = (float)(rx_data[6]);
    motor->Tcoil = (float)(rx_data[7]);
	
}
