#include "bsp.h"
#include "thread_handler.h"
#include "battery_voltage.h"

int main(void)
{  
  InitHardware();
  InitBatteryMeasure();
  
  while(1)
  {   
    LoopThread();
  }
}

