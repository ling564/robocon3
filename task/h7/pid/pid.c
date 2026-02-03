/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       pid.c/h
  * @brief      pid实现函数，包括初始化，PID计算函数，
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#include "pid.h"
#include "h7_motor.h"
#include "MyCAN.h"
#define DEBUG 1
#define LimitMax(VAL, MAX)    do{ if((VAL) > (MAX)) (VAL) = (MAX); }while(0)
#ifndef NULL
#define NULL ((void *)0)
#endif

//PidTypedef pidctrl;

//uint32_t counter = 1;

Pid_Set_Typedef Motor_speed_pid_set = {
			.PID_Init ={0.4,0.00099,0.2},
};

PidTypedef pid_speed;

Pid_Set_Typedef Motor_angle_pid_set = {
			.PID_Init ={1,0.0,0.2},
};
 
PidTypedef pid_angle;

void PID_init(PidTypedef *pid, PID_mode_e mode, const float PID[3], float max_out, float max_iout)
{
	if (pid == NULL || PID == NULL)
	{
		return;
	}
	#if DEBUG
		pid->Kp = PID[0];
		pid->Ki = PID[1];
		pid->Kd = PID[2];
	#endif
	if(pid->Initlized != true)
	{
		pid->pid_mode = mode;
		pid->Kp = PID[0];
		pid->Ki = PID[1];
		pid->Kd = PID[2];
		pid->max_out = max_out;
		pid->max_iout = max_iout;
		pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
		pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
		pid->Initlized = true;
	}
}


float PID_Calc(PidTypedef *pid, float fdb, float ref)
{
    if (pid == NULL)
    {
        return 0.0f;
    }

    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->ref = ref;
    pid->fdb = fdb;
    pid->error[0] = ref - fdb;
	if(pid->Initlized == true)
	{
		switch(pid->pid_mode)
		{
			case PID_POSITION:
			{
				pid->Pout = pid->Kp * pid->error[0];
				pid->Iout += pid->Ki * pid->error[0];
				pid->Dbuf[2] = pid->Dbuf[1];
				pid->Dbuf[1] = pid->Dbuf[0];
				pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
				pid->Dout = pid->Kd * pid->Dbuf[0];
				LimitMax(pid->Iout, pid->max_iout);
				pid->out = pid->Pout + pid->Iout + pid->Dout;
				LimitMax(pid->out, pid->max_out);
			}break;
			
			case PID_DELTA:
			{
				pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
				pid->Iout = pid->Ki * pid->error[0];
				pid->Dbuf[2] = pid->Dbuf[1];
				pid->Dbuf[1] = pid->Dbuf[0];
				pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
				pid->Dout = pid->Kd * pid->Dbuf[0];
				pid->out += pid->Pout + pid->Iout + pid->Dout;
				LimitMax(pid->out, pid->max_out);
			}break;
		}
	}
	else
	{
	    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
		pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
		pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
		pid->fdb = pid->ref = 0.0f;	
	}
    return pid->out;
}

float PID_anglectrl(PidTypedef *pid,float refangle){	
	PID_init(pid,PID_POSITION,Motor_angle_pid_set.PID_Init,5,2.5);
	Motor_angle_pid_set.ref=refangle;
	Motor_angle_pid_set.fdb=txmessage.pos;
	Motor_speed_pid_set.ref=PID_Calc(pid,Motor_angle_pid_set.fdb,Motor_angle_pid_set.ref);
	
	return Motor_speed_pid_set.ref;
}

float PID_speedctrl(PidTypedef *pido){
	float r;
	PID_init(pido,PID_POSITION,Motor_speed_pid_set.PID_Init,12.5,2.5);
	r=PID_Calc(pido,txmessage.vel,Motor_speed_pid_set.ref);
	
	return r;
}

void PID_clear(PidTypedef *pid)
{
    if (pid == NULL)
    {
        return;
    }
	pid->Initlized = false;
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->fdb = pid->ref = 0.0f;
}
