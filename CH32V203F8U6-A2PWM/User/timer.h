#ifndef __TIMER_H
#define __TIMER_H

#include "ch32v20x.h"

// 定时器相关宏定义
#define TIMER_FREQUENCY 1000  // 定时器频率 1000Hz
#define LED_TOGGLE_DIVIDER 500  // 软件分频系数，实现2Hz LED闪烁 (1000/500 = 2Hz)

// 全局变量声明
extern volatile uint16_t timer_counter;
extern volatile uint8_t led_toggle_flag;

// 函数声明
void Timer_Init(void);           // 定时器初始化函数
void TIM1_UP_IRQHandler(void);   // 定时器1更新中断处理函数

#endif /* __TIMER_H */