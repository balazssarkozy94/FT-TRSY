#include "thread_handler.h"

#include "speed_controller.h"
#include "communication_handler.h"

#include "bsp_led.h"

void MilliThread(void)
{
  SpeedControllerHandler();
}

void LoopThread(void)
{
  CommunicationHandler();
  SpeedControllerLoop();
  
  ToggleGreenLed();
}