#ifndef __PWM_H
#define __PWM_H

#include "ch32v20x.h"

// PWM相关宏定义
#define PWM_GPIO_PORT       GPIOA
#define PWM_GPIO_PIN        GPIO_Pin_3
#define PWM_GPIO_CLK        RCC_APB2Periph_GPIOA

#define PWM_TIMER           TIM2
#define PWM_TIMER_CLK       RCC_APB1Periph_TIM2
#define PWM_TIMER_CHANNEL   TIM_Channel_4

// PWM频率和周期相关宏定义
#define PWM_FREQUENCY       20000   // 20KHz
#define PWM_PRESCALER       0       // 预分频器值
#define PWM_PERIOD          3599    // ARR值，实现20KHz频率 (72MHz/(0+1)/(3599+1) = 20KHz)

// PWM占空比相关宏定义
#define PWM_DUTY_MIN        0       // 最小占空比 (0%)
#define PWM_DUTY_MAX        3599    // 最大占空比 (100%)

// 函数声明
void PWM_GPIO_Init(void);           // PWM GPIO初始化
void PWM_Timer_Init(void);          // PWM定时器初始化
void PWM_Module_Init(void);         // PWM模块初始化
void PWM_Set_Duty(uint16_t duty);   // 设置PWM占空比
void PWM_Start(void);               // 启动PWM输出
void PWM_Stop(void);                // 停止PWM输出
uint8_t PWM_Get_Duty_Percent(void); // 获取当前PWM占空比百分比
void PWM_Set_Duty_Percent(uint8_t percent); // 按百分比设置PWM占空比

// 全局变量声明
extern volatile uint16_t pwm_duty;  // 当前PWM占空比值

#endif /* __PWM_H */