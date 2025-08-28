#ifndef __LED_H
#define __LED_H

#include "ch32v20x.h"

// LED引脚定义
#define LED_GPIO_PORT    GPIOB
#define LED_GPIO_PIN     GPIO_Pin_15
#define LED_GPIO_CLK     RCC_APB2Periph_GPIOB

// LED状态宏定义
#define LED_ON()         GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN)    // 高电平点亮LED
#define LED_OFF()        GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN)  // 低电平熄灭LED
#define LED_TOGGLE()     GPIO_WriteBit(LED_GPIO_PORT, LED_GPIO_PIN, \
                                       (BitAction)(1 - GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_GPIO_PIN)))

// 函数声明
void LED_Init(void);             // LED初始化函数
void LED_On(void);               // 点亮LED
void LED_Off(void);              // 熄灭LED
void LED_Toggle(void);           // 切换LED状态

#endif /* __LED_H */