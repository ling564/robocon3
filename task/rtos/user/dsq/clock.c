#include "tim.h"
#include "OLED.h"

void oled_ctrl(void*art){
	int cnt=0;	
	
	while(1){
	OLED_ShowNum(4,4,cnt++,8,OLED_8X16);
	OLED_Update();
	}
	
}
