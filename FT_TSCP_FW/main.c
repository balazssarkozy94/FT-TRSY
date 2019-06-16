#include "bsp.h"
#include "thread_handler.h"

#include "speed_controller.h"
#include "battery_voltage.h"

int main(void)
{    
  InitHardware();
  InitSpeedController();
  InitBatteryMeasure();
  
  while(1)
  {   
    LoopThread();
  }
}

