#include "bsp_pwm_timer.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"

int32_t pwm_value = 0;

void InitPwmTimer(void)
{
  GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
  
    /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  
    /* Time Base configuration */
  TIM1_TimeBaseInit(PWM_TIM_PRESCALER, TIM1_CounterMode_Up, PWM_TIM_PERIOD, PWM_TIM_REPTETION_COUNTER);
  
  TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable, 50, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);
  TIM1_OC2PreloadConfig(DISABLE);
  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM1_Cmd(ENABLE);
  
  SetPwm(0);
}

void SetPwm(int32_t d)
{
  if (d < 0)
    d = 0;
  else if (d > PWM_TIM_PERIOD)
    d = PWM_TIM_PERIOD;
  
  pwm_value = d;
  
  TIM1->CCR2H = d >> 8;
  TIM1->CCR2L = d & 0x00FF;
}

int32_t GetPwm(void)
{
  return pwm_value;
}