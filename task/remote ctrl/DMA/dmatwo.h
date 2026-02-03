#ifndef __DMATEO_H
#define __DMATEO_H

#define SBUS_RX_BUF_NUM 36
#define RC_FRAME_LENGTH 18

#include "usart.h"


extern uint8_t SBUS_MultiRx_Buf[2][18];

extern void USART_RxDMA_DoubleBuffer_Init(UART_HandleTypeDef *huart, uint32_t *DstAddress, uint32_t *SecondMemAddress, uint32_t DataLength);
extern void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size) ;
extern void USER_USART5_RxHandler(UART_HandleTypeDef *huart,uint16_t Size);

#endif
