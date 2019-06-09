#ifndef BSP_PWM_TIMER_H
#define BSP_PWM_TIMER_H

#define PWM_TIM_PERIOD             49999
#define PWM_TIM_PRESCALER          0
#define PWM_TIM_REPTETION_COUNTER  0

#include "stm8l15x_conf.h"

void InitPwmTimer(void);

void SetPwm(int32_t d);

int32_t GetPwm(void);

#endif