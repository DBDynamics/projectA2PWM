/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *USART Print debugging routine:
 *USART1_Tx(PA9).
 *This example demonstrates using USART1(PA9) as a print debug port output.
 *
 */

#include "debug.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "pwm.h"

/* Global typedef */

/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    // USART printf功能已移除
    
    // 初始化LED
    LED_Init();
    // LED初始化完成
    
    // 初始化定时器，启动1000Hz中断和2Hz LED闪烁
    Timer_Init();
    // 定时器初始化完成
    
    // 初始化ADC，启动PA4引脚DMA循环采集
    ADC_Module_Init();
    // ADC初始化完成
    
    // 初始化PWM，启动PA3引脚TIM2 CH4 20KHz PWM输出，初始占空比0%
    PWM_Module_Init();
    // PWM初始化完成

    while(1)
    {
        // 主循环 - ADC值可通过ADC_Get_Value()和ADC_Get_Voltage()函数获取
    }
}
