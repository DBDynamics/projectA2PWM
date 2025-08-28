#ifndef __ADC_H
#define __ADC_H

#include "ch32v20x.h"

// ADC相关宏定义
#define ADC_GPIO_PORT       GPIOA
#define ADC_GPIO_PIN        GPIO_Pin_4
#define ADC_GPIO_CLK        RCC_APB2Periph_GPIOA
#define ADC_CHANNEL         ADC_Channel_4
#define ADC_PERIPH          ADC1
#define ADC_CLK             RCC_APB2Periph_ADC1

// DMA相关宏定义
#define ADC_DMA_CHANNEL     DMA1_Channel1
#define ADC_DMA_CLK         RCC_AHBPeriph_DMA1

// ADC采样参数
#define ADC_SAMPLE_TIME     ADC_SampleTime_239Cycles5  // 采样时间
#define ADC_RESOLUTION      4096                       // 12位ADC分辨率

// 全局变量声明
extern volatile uint16_t adc_value;             // ADC采集值
extern volatile uint8_t adc_conversion_complete; // ADC转换完成标志
extern volatile uint32_t ain;                   // 实际输入电压值(mV)，经过分压电路计算

// 函数声明
void ADC_Module_Init(void);             // ADC模块初始化函数
void ADC_DMA_Init(void);               // ADC DMA初始化函数
void ADC_GPIO_Init(void);              // ADC GPIO初始化函数
void ADC_Start_Conversion(void);       // 启动ADC转换
uint16_t ADC_Get_Value(void);          // 获取ADC值
float ADC_Get_Voltage(void);           // 获取电压值（假设参考电压3.3V）

#endif /* __ADC_H */