#include <string.h>
#include "stm32f10x.h"
#include "led.h"

// 当前板载LED的硬件接线逻辑，编码过程屏蔽接线逻辑
LedHWPin ledHardware[LED_MAX_NUM] = {
    {LED_0, GPIO_Pin_5, GPIOB, RCC_APB2Periph_GPIOB},
    {LED_1, GPIO_Pin_5, GPIOE, RCC_APB2Periph_GPIOE},
};

/**
  * @brief  根据led序号获取对应的pin和gpio，用于屏蔽编码过程中与板载接线逻辑强耦合
  * @param  枚举类LedNum
  * @retval uint16_t* pin, GPIO_TypeDef* gpio
**/
void getLedPinByNum(LedNum num, LedHWPin* ledHwPin)
{
    for (int i = 0; i < LED_MAX_NUM; i++) {
        if (num == ledHardware[i].num) {
            ledHwPin->num = num;
            ledHwPin->pin = ledHardware[i].pin;
            ledHwPin->gpioTypeDef = ledHardware[i].gpioTypeDef;
            ledHwPin->rccGpiox = ledHardware[i].rccGpiox;
        }
    }
}

/**
  * @brief  初始化某个板载LED
  * @param  枚举类LedNum，当参数是LED_MAX_NUM时初始化所有板载LED
  * @retval None
**/
void initSingleLed(LedNum num)
{
    LedHWPin ledHwPin = {0};
    getLedPinByNum(num, &ledHwPin); // 获取led对应硬件引脚

    RCC_APB2PeriphClockCmd(ledHwPin.rccGpiox, ENABLE); // 使能时钟

    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.GPIO_Pin = ledHwPin.pin;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 通用推挽输出
    GPIO_Init(ledHwPin.gpioTypeDef, &gpioInitStruct);
}

/**
  * @brief  对外暴露接口，初始化板载LED
  * @param  枚举类LedNum，当参数是LED_MAX_NUM时初始化所有板载LED
  * @retval None
**/
void initLed(LedNum num)
{
    if (num == LED_MAX_NUM) {
        for (int i = 0; i < LED_MAX_NUM; i++) {
            initSingleLed(i);
        }
    } else {
        initSingleLed(num);
    }
}

void ledOn(LedNum num)
{
    LedHWPin ledHwPin = {0};
    getLedPinByNum(num, &ledHwPin); // 获取led对应硬件引脚

    GPIO_ResetBits(ledHwPin.gpioTypeDef, ledHwPin.pin); // 低电平点亮
}

void ledOff(LedNum num)
{
    LedHWPin ledHwPin = {0};
    getLedPinByNum(num, &ledHwPin); // 获取led对应硬件引脚

    GPIO_SetBits(ledHwPin.gpioTypeDef, ledHwPin.pin); // 高电平熄灭
}

/**
  * @brief  获取LED当前的状态
  * @param  枚举类LedNum
  * @retval 1亮; 0灭
**/
uint8_t isLedOn(LedNum num)
{
    LedHWPin ledHwPin = {0};
    getLedPinByNum(num, &ledHwPin); // 获取led对应硬件引脚

    uint8_t bitStatus = GPIO_ReadInputDataBit(ledHwPin.gpioTypeDef, ledHwPin.pin);
    if (bitStatus == Bit_SET) {
        return 0;
    } else {
        return 1;
    }
}

/**
  * @brief  LED状态翻转，即亮->灭 / 灭->亮
  * @param  枚举类LedNum
  * @retval None
**/
void ledSwitchOver(LedNum num)
{
    LedHWPin ledHwPin = {0};
    getLedPinByNum(num, &ledHwPin); // 获取led对应硬件引脚

    if (isLedOn(num)) {
        ledOff(num);
    } else {
        ledOn(num);
    }
}

