#include "led.h"

/*********************************************************************
 * @fn      LED_Init
 *
 * @brief   初始化LED引脚PB15为推挽输出模式
 *
 * @return  none
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

    // 配置PB15为推挽输出，50MHz速度
    GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 输出速度50MHz
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

    // 初始化时熄灭LED（低电平）
    LED_Off();
}

/*********************************************************************
 * @fn      LED_On
 *
 * @brief   点亮LED（高电平输出）
 *
 * @return  none
 */
void LED_On(void)
{
    GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

/*********************************************************************
 * @fn      LED_Off
 *
 * @brief   熄灭LED（低电平输出）
 *
 * @return  none
 */
void LED_Off(void)
{
    GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

/*********************************************************************
 * @fn      LED_Toggle
 *
 * @brief   切换LED状态（翻转当前输出电平）
 *
 * @return  none
 */
void LED_Toggle(void)
{
    // 读取当前输出状态并翻转
    if (GPIO_ReadOutputDataBit(LED_GPIO_PORT, LED_GPIO_PIN))
    {
        LED_Off();  // 当前为高电平，切换为低电平
    }
    else
    {
        LED_On();   // 当前为低电平，切换为高电平
    }
}