#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

// 板载LED序号
typedef enum _LedNum {
    LED_0 = 0,
    LED_1,
    LED_MAX_NUM
} LedNum;

// 板载LED序号和硬件接线逻辑对应关系
typedef struct _LedHWPin
{
    LedNum num;
    uint16_t pin;
    GPIO_TypeDef* gpioTypeDef;
    uint32_t rccGpiox;
} LedHWPin;

void initLed(LedNum num);
void ledOn(LedNum num);
void ledOff(LedNum num);
void ledSwitchOver(LedNum num);
uint8_t isLedOn(LedNum num);

#endif
