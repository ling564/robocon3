#include "stm32f10x.h"                  // Device header
#include "vofa.h"
#include "Serial.h"
#include "MyCAN.h"
#include "pidr.h"

/**
  * 函数功能：将浮点数拆分成4个字节
  * 入口参数：Fdata：需要操作的浮点数
  * 入口参数：ArrayByte：数组地址
  * 返 回 值：无
  */
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
void JustFloat(float t_f,float r_f)
{
    float a= t_f;
	float b= r_f;
//		Motor_speed_pid_set.fdb;
    uint8_t Byte[4];                               //定义保存浮点数转换后的数组变量
    uint8_t Tail[4] = {0x00, 0x00, 0x80, 0x7f};    //定义包尾数组
    
    Float_to_Byte(a , Byte);                       //转换第一个浮点数
    Serial_SendArray(Byte, 4);  	//发送第一组数据
 
	Float_to_Byte(b , Byte);                       //转换第一个浮点数
    Serial_SendArray(Byte, 4);  
 
    Serial_SendArray(Tail, 4);                     //发送包尾 
    
}
