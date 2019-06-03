#ifndef BSP_LED_H
#define BSP_LED_H

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"

void InitLed(void);

void SetGreenLed(void);

void ResetGreenLed(void);

void ToggleGreenLed(void);

void SetRedLed(void);

void ResetRedLed(void);

void ToggleRedLed(void);

#endif