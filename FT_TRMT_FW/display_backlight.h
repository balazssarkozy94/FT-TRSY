#ifndef DISPLAY_BACKLIGHT_H
#define DISPLAY_BACKLIGHT_H

#define DISPLAY_PWM_MAX         500
#define DISPLAY_PWM_MIN         100
#define DISPLAY_PWM_STEP        100

void IncreaseBrightness(void);

void DecreaseBrightness(void);

void BrightnessHandler(void);

#endif