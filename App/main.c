#include "stm32f10x.h"
#include "led.h"

int main()
{

    initLed(LED_MAX_NUM);
    ledOn(1);
    ledOff(0);

    while (1) {

    }
}