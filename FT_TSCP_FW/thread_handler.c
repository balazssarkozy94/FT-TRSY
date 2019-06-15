#include "thread_handler.h"

#include "speed_controller.h"
#include "communication_handler.h"

void MilliThread(void)
{
  SpeedControllerHandler();
}

void LoopThread(void)
{
  CommunicationHandler();
  SpeedControllerLoop();
}