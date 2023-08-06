#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"

typedef enum _KeyNum {
    KEY_0 = 0,
    KEY_1,
    KEY_MAX_NUM,
} KeyNum;

typedef struct _KeyHWPin
{
    KeyNum num;
    uint16_t pin;
    GPIO_TypeDef* gpioTypeDef;
    uint32_t rccGpiox;
} KeyHWPin;

typedef void (*KeyCallBackFunc)(void);

void regKeyDownCallBack(KeyNum num, void (*callBack)(void));
void regKeyUpCallBack(KeyNum num, void (*callBack)(void));

#endif
