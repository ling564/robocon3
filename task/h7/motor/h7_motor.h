#ifndef __H7_MOTOR_H
#define __H7_MOTOR_H

// 包含必要头文件（保证 uint32_t 可识别）
#include <stdint.h>

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

int float_to_uint(float x_float, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);
void dm_motor_fbdata(motor_t *motor, uint8_t *rx_data);

#endif
