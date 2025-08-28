#include "timer.h"
#include "led.h"

// 全局变量定义
volatile uint16_t timer_counter = 0;
volatile uint8_t led_toggle_flag = 0;

/*********************************************************************
 * @fn      Timer_Init
 *
 * @brief   初始化定时器2，配置为1000Hz中断频率
 *
 * @return  none
 */
void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 使能定时器1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 定时器基本配置
    // 假设系统时钟为72MHz，APB2时钟为72MHz
    // 预分频器设置为71，计数器时钟为1MHz
    // 自动重装载值设置为999，产生1000Hz中断
    TIM_TimeBaseStructure.TIM_Period = 999;                    // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 71;                  // 预分频器值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;           // 重复计数器（高级定时器特有）
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 使能定时器1更新中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // 配置NVIC中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器
    TIM_Cmd(TIM1, ENABLE);
}

// TIM1_UP_IRQHandler函数已移至ch32v20x_it.c文件中