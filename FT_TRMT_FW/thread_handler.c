#include "thread_handler.h"

#include "bsp_led.h"
#include "communication_handler.h"

void MilliThread(void)
{
  
}

void LoopThread(void)
{
  CommunicationHandler();
  ToggleGreenLed();
}