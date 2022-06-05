#include "stm32f10x.h"
#include "key.h"

const KeyHWPin keyHardware[KEY_MAX_NUM] = {
    {KEY_0, GPIO_Pin_4, GPIOE, RCC_APB2Periph_GPIOE},
    {KEY_1, GPIO_Pin_3, GPIOE, RCC_APB2Periph_GPIOE},
};

uint8_t isKeyInit[KEY_MAX_NUM] = {0};
KeyCallBackFunc keyDownCallBack[KEY_MAX_NUM] = {0};
KeyCallBackFunc keyUpCallBack[KEY_MAX_NUM] = {0};

void getKeyPinByNum(KeyNum num, KeyHWPin* keyHwPin)
{
    if (num >= KEY_MAX_NUM) {
        return;
    }

    for (int i = 0; i < KEY_MAX_NUM; i++) {
        if (keyHardware[i].num == num) {
            keyHwPin->num = num;
            keyHwPin->pin = keyHardware[i].pin;
            keyHwPin->gpioTypeDef = keyHardware[i].gpioTypeDef;
            keyHwPin->rccGpiox = keyHardware[i].rccGpiox;
            break;
        }
    }
}

void initKey(KeyNum num)
{
    KeyHWPin keyHwPin;
    getKeyPinByNum(num, &keyHwPin);

    RCC_APB2PeriphClockCmd(keyHwPin.rccGpiox, ENABLE); // 使能时钟

    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.GPIO_Pin = keyHwPin.pin;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(keyHwPin.gpioTypeDef, &gpioInitStruct);
}

void regKeyDownCallBack(KeyNum num, KeyCallBackFunc func)
{
    if (num >= KEY_MAX_NUM) {
        return;
    }

    if (isKeyInit[num] == 0) {
        initKey(num);
    }

    keyDownCallBack[num] = func;
}

void regKeyUpCallBack(KeyNum num, KeyCallBackFunc func)
{
    if (num >= KEY_MAX_NUM) {
        return;
    }
    
    if (isKeyInit[num] == 0) {
        initKey(num);
    }

    keyUpCallBack[num] = func;
}

