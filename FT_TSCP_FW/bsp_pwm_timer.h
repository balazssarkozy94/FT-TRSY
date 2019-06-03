#ifndef BSP_PWM_TIMER_H
#define BSP_PWM_TIMER_H

#define TIM1_PERIOD             499
#define TIM1_PRESCALER          0
#define TIM1_REPTETION_COUNTER  0

void InitPwmTimer(void);

void SetPwm(int d);

#endif