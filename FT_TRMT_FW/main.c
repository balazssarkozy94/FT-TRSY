#include "bsp.h"
#include "thread_handler.h"

#include "stm8l15x_gpio.h"
#include "bsp_led.h"
#include "bsp_timing_delay.h"

int main(void)
{  
  InitHardware();
  
  while(1)
  {   
    LoopThread();
  }
}

