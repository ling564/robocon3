#ifndef __DATA_H
#define __DATA_H

#include <stdint.h>

typedef struct
{
 struct
 {
 uint16_t ch0; uint16_t ch1; uint16_t ch2; uint16_t ch3; uint8_t s1; uint8_t s2; }rc;
 struct
 {
 int16_t x;
 int16_t y;
 int16_t z;
 uint8_t press_l; uint8_t press_r; }mouse;
 struct
 {
 uint16_t v;
 }key;
}RC_Ctl_t;

extern RC_Ctl_t RC_CtrlData;

extern void RemoteDataProcess(RC_Ctl_t*remote,uint8_t *pData);

#endif
