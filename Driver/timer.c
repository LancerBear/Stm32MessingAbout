#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "timer.h"

/**
 * TIM1 TIM8属于高级控制定时器, TIM2 3 4 5通用控制定时器, TIM6 7基本控制定时器, 此处使用TIM2
*/
void initTimer()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_Prescaler = 7200 - 1, /* 10k Hz, 16位寄存器0-65535之间*/
        .TIM_Period = 10000 - 1, /* 10k Hz计数100次, 10ms触发中断 */
        .TIM_ClockDivision = TIM_CKD_DIV1,
        .TIM_RepetitionCounter = 0, /* 高级定时器才有, 此处忽略 */
    };

    NVIC_InitTypeDef NVIC_InitStruct = {
        .NVIC_IRQChannel = TIM2_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 2,
        .NVIC_IRQChannelSubPriority = 1,
        .NVIC_IRQChannelCmd = ENABLE,
    };

    // TIM2挂在APB1总线,使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 选择内部时钟
    TIM_InternalClockConfig(TIM2);

    // 配置时钟
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // 使能中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 配置中断控制
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM2, ENABLE);
}
