#include "display_backlight.h"

#include "bsp_pwm_timer.h"
#include <inttypes.h>

uint16_t DisplayPwm = DISPLAY_PWM_MAX;

void IncreaseBrightness(void)
{
  DisplayPwm += DISPLAY_PWM_STEP;
  
  if (DisplayPwm > DISPLAY_PWM_MAX)
    DisplayPwm = DISPLAY_PWM_MAX;
}

void DecreaseBrightness(void)
{
  DisplayPwm -= DISPLAY_PWM_STEP;
  
  if (DisplayPwm < DISPLAY_PWM_MIN)
    DisplayPwm = DISPLAY_PWM_MIN;
}

void BrightnessHandler(void)
{
  SetPwm(DisplayPwm);
}