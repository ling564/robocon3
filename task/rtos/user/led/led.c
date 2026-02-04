#include "gpio.h"


void led_ctrl(void){
	while(1){
/* 点亮LED */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
/* 延时 */
HAL_Delay(500); // 延时500ms
/* 熄灭LED */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
/* 延时 */
HAL_Delay(500); // 延时500ms
	}
}