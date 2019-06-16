#include "thread_handler.h"

#include "speed_controller.h"
#include "communication_handler.h"
#include "battery_voltage.h"

#include "bsp_led.h"
#include "bsp_timing_delay.h"

void MilliThread(void)
{
  SpeedControllerHandler();
  BatteryMeasureTimeHandler();
}

void LoopThread(void)
{
  CommunicationHandler();
  SpeedControllerLoop();
  BatteryMeasureHandler();
  
  SetGreenLed();
  Delay(1);
  ResetGreenLed();
}