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
#ifndef PID_R_H
#define PID_R_H                 
#include "stdbool.h"
#include <stdint.h>
typedef	enum
{
    PID_POSITION = 0,
    PID_DELTA
}PID_mode_e;

typedef struct
{
    PID_mode_e pid_mode;
	bool	Initlized; 
    //PID 三参数
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //最大输出
    float max_iout; //最大积分输出

    float ref;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];  //微分项 0最新 1上一次 2上上次
    float error[3]; //误差项 0最新 1上一次 2上上次

} PidTypedef;

typedef struct
{
	
	float ref;
	float fdb;
	float PID_Init[3];

} Pid_Set_Typedef;

extern void PID_init(PidTypedef *pid, PID_mode_e mode, const float PID[3], float max_out, float max_iout);
extern float PID_Calc(PidTypedef *pid, float fdb, float ref);
extern void PID_clear(PidTypedef *pid);
extern float PID_anglectrl(PidTypedef *pid,float refangle);
extern float PID_speedctrl(PidTypedef *pid);
extern Pid_Set_Typedef Motor_speed_pid_set;
extern Pid_Set_Typedef Motor_angle_pid_set;
extern PidTypedef pid_speed;
extern PidTypedef pid_angle;

#endif
