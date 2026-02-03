#include "stm32f10x.h"                  // Device header
#include "pid.h"

float PID_Compute(float setpoint, float current_speed)
{   	
	float previous_error = 0;
	float integral = 0;
	float error = setpoint - current_speed;
	integral += error;
	float derivative = error - previous_error;	
	float output = KP * error + KI * integral + KD * derivative;	
	previous_error = error;
	
    return output;
}
