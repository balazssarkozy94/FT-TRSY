#include "thread_handler.h"

#include "bsp_input_gpio.h"
#include "bsp_led.h"

#include "communication_handler.h"
#include "display_handler.h"
#include "battery_voltage.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"

void MilliThread(void)
{
  ButtonFilterTimeHandler();
  DisplayTimeHandler();
  BatteryMeasureTimeHandler();
}

void LoopThread(void)
{
  CommunicationHandler();  
  DisplayHandler();
  BatteryMeasureHandler();
  
  SetGreenLed();
  ResetGreenLed();
}