#include "MyCAN.h"
#include "h7_motor.h"
#include "fdcan.h"
#include "main.h"

DM_Motor_Info_Typedef CANFrameInfo={
	.TxStdId=0x02,
	
};

motor_t txmessage={

	.PMAX=12.5,
	.VMAX=30.0,
	.TMAX=10.0,
};

FDCAN_RxFrame_TypeDef FDCAN1_RxFrame;

FDCAN_TxFrame_TypeDef hfdcan1TxFrame = {
    .hcan = &hfdcan1,                          // 使用CAN1发送
    .Header.IdType = FDCAN_STANDARD_ID,        // 标准ID
    .Header.TxFrameType = FDCAN_DATA_FRAME,    // 数据帧
    .Header.DataLength = 8,                    // 8字节
    .Header.ErrorStateIndicator = FDCAN_ESI_ACTIVE, // CAN发送错误指示
    .Header.BitRateSwitch = FDCAN_BRS_OFF,     // 波特率切换关闭
    .Header.FDFormat = FDCAN_CLASSIC_CAN,      // 经典CAN模式
    .Header.TxEventFifoControl = FDCAN_NO_TX_EVENTS, // 不储存发送事件
    .Header.MessageMarker = 0,                 // 消息标记0
};

void FDCAN1_Config_Filter(void){
	
	FDCAN_FilterTypeDef FDCAN1_FilterConfig;

// 配置过滤器类型为标准ID（经典CAN仅支持标准ID）
FDCAN1_FilterConfig.IdType = FDCAN_STANDARD_ID;
// 过滤器编号，使用多路CAN时依次为0、1、2...
FDCAN1_FilterConfig.FilterIndex = 0;
// 过滤器模式为MASK模式，会影响后续FilterID1和FilterID2的配置逻辑
FDCAN1_FilterConfig.FilterType = FDCAN_FILTER_MASK;
// 将过滤器映射到FIFO0接收；若需映射到FIFO1，改为FDCAN_FILTER_TO_RXFIFO1
FDCAN1_FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
// FilterID1可任意设置，只要FilterID2也设为0x00000000，就会接收所有ID的报文
FDCAN1_FilterConfig.FilterID1 = 0x00000000;
// FilterID2设为0x00000000，配合FilterID1=0实现无过滤（接收所有报文）
FDCAN1_FilterConfig.FilterID2 = 0x00000000;

// 将上述过滤器配置写入FDCAN1外设
HAL_FDCAN_ConfigFilter(&hfdcan1, &FDCAN1_FilterConfig);

// 配置全局过滤规则：拒绝未被过滤器匹配的报文，同时过滤远程帧
HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);

// 激活FIFO0的新报文接收中断通知
HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

// 启动FDCAN1控制器
HAL_FDCAN_Start(&hfdcan1);
}

void DM_Motor_Enable(FDCAN_TxFrame_TypeDef *TxFrame, DM_Motor_Info_Typedef *DM_Motor){

    TxFrame->Header.Identifier = DM_Motor->TxStdId; 

    TxFrame->Data[0] = 0xFF;
    TxFrame->Data[1] = 0xFF;
    TxFrame->Data[2] = 0xFF;
    TxFrame->Data[3] = 0xFF;
    TxFrame->Data[4] = 0xFF;
    TxFrame->Data[5] = 0xFF;
    TxFrame->Data[6] = 0xFF;
    TxFrame->Data[7] = 0xFC;

    HAL_FDCAN_AddMessageToTxFifoQ(TxFrame->hcan,&TxFrame->Header, TxFrame->Data);

}

void motor_ctrl(FDCAN_TxFrame_TypeDef *TxFrame,DM_Motor_Info_Typedef *DM_Motor,motor_t *motor,float pos, float vel,float kp, float kd, float tor){
	
	 TxFrame->Header.Identifier = DM_Motor->TxStdId;
	
    uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;

    pos_tmp = float_to_uint(pos, -motor->PMAX, motor->PMAX, 16);
    vel_tmp = float_to_uint(vel, -motor->VMAX, motor->VMAX, 12);
    tor_tmp = float_to_uint(tor, -motor->TMAX, motor->TMAX, 12);
    kp_tmp  = float_to_uint(kp,  0, 500, 12);
    kd_tmp  = float_to_uint(kd,  0, 5, 12);

    TxFrame->Data[0]  = (pos_tmp >> 8);
    TxFrame->Data[1]  = pos_tmp;
    TxFrame->Data[3]  = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
    TxFrame->Data[2]  = (vel_tmp >> 4);
    TxFrame->Data[4]  = kp_tmp;
    TxFrame->Data[5]  = (kd_tmp >> 4);
    TxFrame->Data[6]  = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
    TxFrame->Data[7]  = tor_tmp;
	
	HAL_FDCAN_AddMessageToTxFifoQ(TxFrame->hcan,&TxFrame->Header, TxFrame->Data);
	
}

void FDCAN1_RxFifo0RxHandler(motor_t*txmotor,uint8_t Data[8]){
	
	dm_motor_fbdata(txmotor, Data);

}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_RxFrame.Header, FDCAN1_RxFrame.Data);
	if(FDCAN1_RxFrame.Header.Identifier==0x12){
    FDCAN1_RxFifo0RxHandler(&txmessage, FDCAN1_RxFrame.Data);
	}
}
