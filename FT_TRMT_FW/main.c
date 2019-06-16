#include "bsp.h"
#include "thread_handler.h"

int main(void)
{  
  InitHardware();
  
  while(1)
  {   
    LoopThread();
  }
}

