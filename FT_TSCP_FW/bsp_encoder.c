#include "bsp_encoder.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"

void InitEncoder(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_In_PU_No_IT);
  
    /* Enable TIM3 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  
  /* Time Base configuration */
  TIM3_TimeBaseInit(TIM3_Prescaler_1, TIM3_CounterMode_Up, 0xFFFF);
  
  TIM3_ETRClockMode1Config(TIM3_ExtTRGPSC_OFF, TIM3_ExtTRGPolarity_NonInverted, 0x01);
 
  TIM3_Cmd(ENABLE);
}