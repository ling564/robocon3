#include "data.h"

RC_Ctl_t RC_CtrlData;

void RemoteDataProcess(RC_Ctl_t*remote,uint8_t *pData)
{
// if(pData == 0)
// {
// return;
// }
 
 remote->rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;  
 remote->rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF; 
 remote->rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) | ((int16_t)pData[4] << 10)) & 0x07FF;
 remote->rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;
 
 remote->rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2; 
 remote->rc.s2 = ((pData[5] >> 4) & 0x0003);
 remote->mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8); 
 remote->mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8); 
 remote->mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8); 
 remote->mouse.press_l = pData[12]; 
 remote->mouse.press_r = pData[13];
 remote->key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);
 
}
