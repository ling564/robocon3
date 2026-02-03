#include "dmatwo.h"
#include "data.h"

uint8_t SBUS_MultiRx_Buf[2][18]={};

void USART_RxDMA_DoubleBuffer_Init(UART_HandleTypeDef *huart, uint32_t *DstAddress, uint32_t *SecondMemAddress, uint32_t DataLength){ 

 huart->ReceptionType = HAL_UART_RECEPTION_TOIDLE; 

 huart->RxEventType = HAL_UART_RXEVENT_IDLE; 

 huart->RxXferSize    = DataLength; 

 SET_BIT(huart->Instance->CR3,USART_CR3_DMAR); 

 __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);  
 
 HAL_DMAEx_MultiBufferStart(huart->hdmarx,(uint32_t)&huart->Instance->RDR,(uint32_t)DstAddress,(uint32_t)SecondMemAddress,DataLength); 
 }

void USER_USART5_RxHandler(UART_HandleTypeDef *huart,uint16_t Size){ 

 if(((((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR) & DMA_SxCR_CT ) == RESET) 
 { 
	  __HAL_DMA_DISABLE(huart->hdmarx); 

	  ((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR |= DMA_SxCR_CT; 
 
	  __HAL_DMA_SET_COUNTER(huart->hdmarx,SBUS_RX_BUF_NUM); 

	  if(Size == RC_FRAME_LENGTH) 
	  { 
		RemoteDataProcess(&RC_CtrlData,SBUS_MultiRx_Buf[0]);
	  } 
 
 }else{ 
	 __HAL_DMA_DISABLE(huart->hdmarx); 

	  ((DMA_Stream_TypeDef  *)huart->hdmarx->Instance)->CR &= ~(DMA_SxCR_CT); 
 
	  __HAL_DMA_SET_COUNTER(huart->hdmarx,SBUS_RX_BUF_NUM); 

	  if(Size == RC_FRAME_LENGTH) 
	  { 
		RemoteDataProcess(&RC_CtrlData,SBUS_MultiRx_Buf[1]);
	  }			 
 } 
		 __HAL_DMA_ENABLE(huart->hdmarx);				 
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size) 
{ 
	 if(huart == &huart5){ 
 
		 USER_USART5_RxHandler(huart,Size); 
 
	 }  
}
