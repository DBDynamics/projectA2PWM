#include "adc.h"

// 全局变量定义
volatile uint16_t adc_value = 0;                // ADC采集值
volatile uint8_t adc_conversion_complete = 0;   // ADC转换完成标志
volatile uint32_t ain = 0;                      // 实际输入电压值(mV)，经过分压电路计算

/*********************************************************************
 * @fn      ADC_GPIO_Init
 *
 * @brief   初始化ADC GPIO引脚PA4为模拟输入模式
 *
 * @return  none
 */
void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(ADC_GPIO_CLK, ENABLE);

    // 配置PA4为模拟输入模式
    GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    // 模拟输入模式
    GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      ADC_DMA_Init
 *
 * @brief   初始化DMA用于ADC数据传输
 *
 * @return  none
 */
void ADC_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 使能DMA1时钟
    RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);

    // DMA通道配置
    DMA_DeInit(ADC_DMA_CHANNEL);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->RDATAR;  // ADC数据寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_value;        // 内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  // 从外设到内存
    DMA_InitStructure.DMA_BufferSize = 1;                               // 传输数据量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;            // 内存地址不递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16位数据
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 16位数据
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                     // 循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // 高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // 非内存到内存
    DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);

    // 配置DMA中断
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能DMA传输完成中断
    DMA_ITConfig(ADC_DMA_CHANNEL, DMA_IT_TC, ENABLE);

    // 使能DMA通道
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
}

/*********************************************************************
 * @fn      ADC_Module_Init
 *
 * @brief   初始化ADC1，配置为连续转换模式
 *
 * @return  none
 */
void ADC_Module_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};

    // 初始化GPIO
    ADC_GPIO_Init();

    // 初始化DMA
    ADC_DMA_Init();

    // 使能ADC1时钟
    RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);

    // 配置ADC时钟分频
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // ADC时钟 = PCLK2/6 = 72MHz/6 = 12MHz

    // ADC基本配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                    // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                        // 单通道，关闭扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                  // 单次转换模式，由定时器触发
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                              // 1个转换通道
    ADC_Init(ADC_PERIPH, &ADC_InitStructure);

    // 配置ADC通道4（PA4）
    ADC_RegularChannelConfig(ADC_PERIPH, ADC_CHANNEL, 1, ADC_SAMPLE_TIME);

    // 使能ADC DMA请求
    ADC_DMACmd(ADC_PERIPH, ENABLE);

    // 使能ADC
    ADC_Cmd(ADC_PERIPH, ENABLE);

    // ADC校准
    ADC_ResetCalibration(ADC_PERIPH);
    while(ADC_GetResetCalibrationStatus(ADC_PERIPH));
    ADC_StartCalibration(ADC_PERIPH);
    while(ADC_GetCalibrationStatus(ADC_PERIPH));

    // ADC初始化完成，等待定时器触发转换
}

/*********************************************************************
 * @fn      ADC_Start_Conversion
 *
 * @brief   启动ADC转换
 *
 * @return  none
 */
void ADC_Start_Conversion(void)
{
    ADC_SoftwareStartConvCmd(ADC_PERIPH, ENABLE);
}

/*********************************************************************
 * @fn      ADC_Get_Value
 *
 * @brief   获取ADC采集值
 *
 * @return  ADC采集的原始值（0-4095）
 */
uint16_t ADC_Get_Value(void)
{
    return adc_value;
}

/*********************************************************************
 * @fn      ADC_Get_Voltage
 *
 * @brief   获取ADC对应的电压值
 *
 * @return  电压值（单位：V）
 */
float ADC_Get_Voltage(void)
{
    // 假设参考电压为3.3V，12位ADC分辨率为4096
    return (float)adc_value * 3.3f / 4096.0f;
}

// DMA1_Channel1_IRQHandler 中断处理函数已移至 ch32v20x_it.c 文件中