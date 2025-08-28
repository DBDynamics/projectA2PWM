/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v20x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/29
 * Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v20x_it.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "pwm.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  NVIC_SystemReset();
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      TIM1_UP_IRQHandler
 *
 * @brief   定时器1更新中断处理函数，实现2Hz软件分频控制LED闪烁
 *
 * @return  none
 */
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        // 清除中断标志
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        
        // 触发ADC转换（1000Hz采样频率）
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        
        // 计数器递增
        timer_counter++;
        
        // 软件分频：每500次中断（500ms）切换一次LED状态，实现2Hz闪烁
        if (timer_counter >= LED_TOGGLE_DIVIDER)
        {
            timer_counter = 0;          // 重置计数器
            led_toggle_flag = 1;        // 设置LED切换标志
            LED_Toggle();               // 切换LED状态
        }
    }
}

/*********************************************************************
 * @fn      DMA1_Channel1_IRQHandler
 *
 * @brief   This function handles DMA1 Channel1 interrupt.
 *
 * @return  none
 */
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
    {
        // 清除中断标志
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        
        // 计算实际输入电压值(mV): ain = adc_value * 3300 / 4096 * 11 / 1
        ain = (uint32_t)adc_value * 3300 * 11 / 4096;
        
        // 根据ain值设置PWM占空比
        if (ain > 10000)
        {
            // 如果ain > 10000mV，设置PWM占空比为100%
            PWM_Set_Duty_Percent(100);
        }
        else
        {
            // PWM占空比 = ain / 10000 * 100%
            uint8_t duty_percent = (uint8_t)(ain / 100);  // ain/10000*100 = ain/100
            PWM_Set_Duty_Percent(duty_percent);
        }
        
        // 设置转换完成标志
        adc_conversion_complete = 1;
    }
}


