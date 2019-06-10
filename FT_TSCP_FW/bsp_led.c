#include "bsp_led.h"

void InitLed(void)
{
  GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast);
}

void SetGreenLed(void)
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}

void ResetGreenLed(void)
{
  GPIO_SetBits(GPIOC, GPIO_Pin_1);
}

void ToggleGreenLed(void)
{
  GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
}

void SetRedLed(void)
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}

void ResetRedLed(void)
{
  GPIO_SetBits(GPIOC, GPIO_Pin_0);
}

void ToggleRedLed(void)
{
  GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
}
