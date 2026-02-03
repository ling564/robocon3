#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"                  // Device header

typedef struct
{    
    uint8_t id;
    uint8_t state;
    uint32_t p_int;
    uint32_t v_int; 
    uint32_t t_int; 

    float pos;
    float vel;
    float tor;
	
    float Tmos;
    float Tcoil;
	
	float PMAX;
	float VMAX;
	float TMAX;

} motor_t;

extern motor_t dm_4310_rxdata;
extern motor_t tx_4310;
void motor_ctrl(motor_t *motor, uint16_t motor_id, float pos, float vel,float kp, float kd, float tor);
int float_to_uint(float x_float, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void dm_motor_fbdata(motor_t *motor, uint8_t *rx_data);
#endif
