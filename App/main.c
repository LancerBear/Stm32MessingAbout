#include "stm32f10x.h"
#include "led.h"
#include "timer.h"

int main()
{

    initLed(LED_MAX_NUM);
    initTimer();

    while (1);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != SET) {
        return;
    }

    ledSwitchOver(1);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}