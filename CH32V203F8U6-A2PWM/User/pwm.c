#include "pwm.h"

// 全局变量定义
volatile uint16_t pwm_duty = 0;  // 当前PWM占空比值，初始为0

/*********************************************************************
 * @fn      PWM_GPIO_Init
 *
 * @brief   初始化PWM输出引脚PA3
 *
 * @return  none
 */
void PWM_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(PWM_GPIO_CLK, ENABLE);
    
    // 配置PA3为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = PWM_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 50MHz输出速度
    GPIO_Init(PWM_GPIO_PORT, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      PWM_Timer_Init
 *
 * @brief   初始化TIM2定时器用于PWM输出
 *
 * @return  none
 */
void PWM_Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    
    // 使能TIM2时钟
    RCC_APB1PeriphClockCmd(PWM_TIMER_CLK, ENABLE);
    
    // 配置定时器基本参数
    TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD;              // ARR = 3599
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALER;        // PSC = 0
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(PWM_TIMER, &TIM_TimeBaseStructure);
    
    // 配置PWM输出通道4
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;           // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   // 高电平有效
    TIM_OCInitStructure.TIM_Pulse = 0;                          // 初始占空比为0
    TIM_OC4Init(PWM_TIMER, &TIM_OCInitStructure);
    
    // 使能通道4预装载
    TIM_OC4PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    
    // 使能定时器ARR预装载
    TIM_ARRPreloadConfig(PWM_TIMER, ENABLE);
}

/*********************************************************************
 * @fn      PWM_Module_Init
 *
 * @brief   PWM模块初始化
 *
 * @return  none
 */
void PWM_Module_Init(void)
{
    // 初始化PWM GPIO
    PWM_GPIO_Init();
    
    // 初始化PWM定时器
    PWM_Timer_Init();
    
    // 设置初始占空比为0
    PWM_Set_Duty(PWM_DUTY_MIN);
    
    // 启动PWM输出
    PWM_Start();
}

/*********************************************************************
 * @fn      PWM_Set_Duty
 *
 * @brief   设置PWM占空比
 *
 * @param   duty - 占空比值 (0-3599)
 *
 * @return  none
 */
void PWM_Set_Duty(uint16_t duty)
{
    // 限制占空比范围
    if (duty > PWM_DUTY_MAX)
    {
        duty = PWM_DUTY_MAX;
    }
    
    // 更新全局变量
    pwm_duty = duty;
    
    // 设置比较值
    TIM_SetCompare4(PWM_TIMER, duty);
}

/*********************************************************************
 * @fn      PWM_Start
 *
 * @brief   启动PWM输出
 *
 * @return  none
 */
void PWM_Start(void)
{
    // 使能定时器
    TIM_Cmd(PWM_TIMER, ENABLE);
}

/*********************************************************************
 * @fn      PWM_Stop
 *
 * @brief   停止PWM输出
 *
 * @return  none
 */
void PWM_Stop(void)
{
    // 禁用定时器
    TIM_Cmd(PWM_TIMER, DISABLE);
}

/*********************************************************************
 * @fn      PWM_Get_Duty_Percent
 *
 * @brief   获取当前PWM占空比百分比
 *
 * @return  占空比百分比 (0-100)
 */
uint8_t PWM_Get_Duty_Percent(void)
{
    return (uint8_t)((pwm_duty * 100) / PWM_PERIOD);
}

/*********************************************************************
 * @fn      PWM_Set_Duty_Percent
 *
 * @brief   按百分比设置PWM占空比
 *
 * @param   percent - 占空比百分比 (0-100)
 *
 * @return  none
 */
void PWM_Set_Duty_Percent(uint8_t percent)
{
    uint16_t duty;
    
    // 限制百分比范围
    if (percent > 100)
    {
        percent = 100;
    }
    
    // 计算对应的占空比值
    duty = (uint16_t)((percent * PWM_PERIOD) / 100);
    
    // 设置占空比
    PWM_Set_Duty(duty);
}