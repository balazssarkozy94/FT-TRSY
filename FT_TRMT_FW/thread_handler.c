#include "thread_handler.h"

#include "bsp_led.h"
#include "communication_handler.h"
#include "display_backlight.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"

void MilliThread(void)
{
  
}

void LoopThread(void)
{
  CommunicationHandler();
  BrightnessHandler();
}