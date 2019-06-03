#include "bsp_itc_config.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_it.h"

void ITC_Config(void)
{
  ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_IRQn, ITC_PriorityLevel_1);
  ITC_SetSoftwarePriority(EXTI3_IRQn, ITC_PriorityLevel_3);
  ITC_SetSoftwarePriority(EXTI4_IRQn, ITC_PriorityLevel_3);
  ITC_SetSoftwarePriority(EXTI5_IRQn, ITC_PriorityLevel_3);
  ITC_SetSoftwarePriority(EXTI6_IRQn, ITC_PriorityLevel_3);
}