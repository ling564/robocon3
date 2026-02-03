#ifndef __VOFA_H
#define __VOFA_H
#include "stm32f10x.h"  


/*定义共同体变量类型*/
typedef union
{
    float Fdata;         //以浮点数形式读取变量
    uint64_t Adata;      //以32位无符号形式读取变量
    
}Vofa_Type;

void Float_to_Byte(float Fdata,  uint8_t *ArrayByte);
void JustFloat(float t_f,float r_f);

#endif
