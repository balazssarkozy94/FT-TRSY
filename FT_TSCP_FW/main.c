#include "bsp.h"
#include "thread_handler.h"

#include "speed_controller.h"

int main(void)
{    
  InitHardware();
  InitSpeedController();
  
  while(1)
  {   
    LoopThread();
  }
}

