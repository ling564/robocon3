#ifndef __MYCAN_H
#define __MYCAN_H
#include "stm32f10x.h"    

extern CanRxMsg MyCAN_RxMsg;
extern uint8_t MyCAN_RxFlag;

void MyCAN_Init(void);
void MyCAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data);
void MyCAN_Receive(uint32_t *ID, uint8_t *Length, uint8_t *Data);

#endif
