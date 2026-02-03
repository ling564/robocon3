#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "pidr.h"
#include "vofa.h"
#include "MyCAN.h"
#include "Serial.h"
#include "motor.h"



float c;
uint8_t Data_1111[8]={0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF, 0xFF, 0xFC};

//CanTxMsg TxMsgArray[] = {
///*   StdId     ExtId         IDE             RTR        DLC         Data[8]          */
//	{0x002, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   8, {0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF, 0xFF, 0xFC}},
////	{0x002, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   8, {0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00|t_ff>>8, t_ff<<4}},

//};

PidTypedef Motor_speed_pid;
Pid_Set_Typedef Motor_speed_pid_set = {
			.PID_Init ={0.1,0.0,0.0},
};

int main(void)
{	
	MyCAN_Init();
	Serial_Init();
    
//	CanTxMsg TxMessage;
//	TxMessage.StdId = 0x002;
//	TxMessage.ExtId = 0x00000000;
//	TxMessage.IDE = CAN_Id_Standard;		//CAN_ID_STD
//	TxMessage.RTR = CAN_RTR_Data;
//	TxMessage.DLC =8;
//	for (uint8_t i = 0; i < 8; i ++)
//	{
//		TxMessage.Data[i] = Data[i];
//	}

	
	
	
	//Delay_ms(5);

	MyCAN_Transmit(0x02,8,Data_1111);
	
//	MyCAN_Transmit(&TxMsgArray[1]);
    
	Motor_speed_pid_set.ref = 3;
	while(1)
	{	
		PID_init(&Motor_speed_pid,PID_POSITION,Motor_speed_pid_set.PID_Init,12.5,10);
		Motor_speed_pid_set.fdb = dm_4310_rxdata.vel ;
		
		c=PID_Calc(&Motor_speed_pid,Motor_speed_pid_set.fdb,Motor_speed_pid_set.ref);
		
		motor_ctrl(&tx_4310, 0x02, 0, 0,0, 0, c);
		Delay_ms(10);
		
//		if(MyCAN_RxFlag == 1){
			
		JustFloat(Motor_speed_pid_set.ref,Motor_speed_pid_set.fdb);
			
//		MyCAN_RxFlag=0;
//		}
//		Motor_speed_pid_set.ref = 0;
//		Motor_speed_pid_set.fdb = motor->para.vel;
//		MyCAN_Transmit(&TxMsgArray[0]);
//		Delay_ms(5);
//		t = PID_Calc(&Motor_speed_pid,Motor_speed_pid_set.fdb,Motor_speed_pid_set.ref);
	}
}
