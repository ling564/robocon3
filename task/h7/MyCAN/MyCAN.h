#ifndef __MYCAN_H
#define __MYCAN_H

// 1. 先包含依赖头文件（保证所有类型可提前识别）
#include "fdcan.h"
#include "h7_motor.h"
#include <stdint.h>

typedef struct{
    uint32_t TxStdId;// CAN发送标准帧ID（如大疆电机 0x200、0x1FF）
	
} DM_Motor_Info_Typedef;

// 2. 先定义 FDCAN_TxFrame_TypeDef 结构体（先定义，后使用）
typedef struct{
    FDCAN_HandleTypeDef *hcan;
    FDCAN_TxHeaderTypeDef Header;
    uint8_t Data[8];
}FDCAN_TxFrame_TypeDef;

typedef struct {
    FDCAN_HandleTypeDef *hcan;
    FDCAN_RxHeaderTypeDef Header;
    uint8_t Data[8];
} FDCAN_RxFrame_TypeDef;


// 3. 声明函数（此时结构体已定义，编译器可识别）
void FDCAN1_Config_Filter(void);
void DM_Motor_Enable(FDCAN_TxFrame_TypeDef *TxFrame, DM_Motor_Info_Typedef *DM_Motor);
void motor_ctrl(FDCAN_TxFrame_TypeDef *TxFrame,DM_Motor_Info_Typedef *DM_Motor,motor_t *motor,float pos, float vel,float kp, float kd, float tor);
void FDCAN1_RxFifo0RxHandler(motor_t*txmotor,uint8_t Data[8]);
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

extern DM_Motor_Info_Typedef CANFrameInfo;
extern FDCAN_TxFrame_TypeDef hfdcan1TxFrame;
extern motor_t txmessage;
extern FDCAN_RxFrame_TypeDef FDCAN1_RxFrame;

#endif
