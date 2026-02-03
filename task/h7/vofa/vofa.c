#include "vofa.h"
#include "usart.h"

float Vofa_buff[10] = {0};


void Float_to_Byte(float Fdata,  uint8_t *ArrayByte)
{
    
    Vofa_Type Vofa;                   //定义Vofa_Type类型的Vofa变量
    
    Vofa.Fdata = Fdata;               //把需要操作的浮点数复制到共同体的Fdata变量中
    ArrayByte[0]=Vofa.Adata;  //0-7位移到数组元素0
    ArrayByte[1]=Vofa.Adata>>8; //8-15位移动到数组元素1
    ArrayByte[2]=Vofa.Adata>>16; //16-23位移动到数组元素2
    ArrayByte[3]=Vofa.Adata>>24;  //24-31位移动到数组元素3 
    
	
}

/**
  * 函数功能：串口发送数据到VOFA+上位机
  * 入口参数：无
  * 返 回 值：无
  */
void JustFloat(float *buff,uint8_t num)
{
    uint8_t Byte[4];                               //定义保存浮点数转换后的数组变量
    uint8_t Tail[4] = {0x00, 0x00, 0x80, 0x7f};    //定义包尾数组
    for(uint8_t i=0;i<num;i++)
	{
	    Float_to_Byte(buff[i] , Byte);                       //转换第1个浮点数
		HAL_UART_Transmit(&huart1, Byte, 4, 0xFFFF);  	
	}
    HAL_UART_Transmit(&huart1, Tail, 4, 0xFFFF);                     //发送包尾 
    
}
