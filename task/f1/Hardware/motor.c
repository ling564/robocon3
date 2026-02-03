#include "stm32f10x.h"                  // Device header
#include "pidr.h"
#include "MyCAN.h"
#include "motor.h"
	
motor_t dm_4310_rxdata={

	.PMAX=12.5,
	.VMAX=30.0,
	.TMAX=10.0,
};
motor_t tx_4310={

	.PMAX=12.5,
	.VMAX=30.0,
	.TMAX=10.0,
};
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

void motor_ctrl(motor_t *motor, uint16_t motor_id, float pos, float vel,float kp, float kd, float tor){
	
	uint8_t data[8];
    uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;
    uint16_t id = motor_id + 0x00;

    pos_tmp = float_to_uint(pos, -motor->PMAX, motor->PMAX, 16);
    vel_tmp = float_to_uint(vel, -motor->VMAX, motor->VMAX, 12);
    tor_tmp = float_to_uint(tor, -motor->TMAX, motor->TMAX, 12);
    kp_tmp  = float_to_uint(kp,  0, 500, 12);
    kd_tmp  = float_to_uint(kd,  0, 5, 12);

    data[0] = (pos_tmp >> 8);
    data[1] = pos_tmp;
    data[2] = (vel_tmp >> 4);
    data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
    data[4] = kp_tmp;
    data[5] = (kd_tmp >> 4);
    data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
    data[7] = tor_tmp;
	
	MyCAN_Transmit(0x02,8,data);
	
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
