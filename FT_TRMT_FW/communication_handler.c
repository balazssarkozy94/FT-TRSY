#include "communication_handler.h"

#include "bsp_timing_delay.h"
#include "bsp_rf_com.h"
#include "bsp_led.h"

#include "display_handler.h"
#include "ft_remote_types.h"

TelescopeMessageType TelescopeInputMessage;
uint8_t len = sizeof(TelescopeMessageType);

void CommunicationHandler(void)
{
  if(recv((uint8_t*)&TelescopeInputMessage, &len))
  {              
    DisplayMessageArrived(&TelescopeInputMessage);
    
    SetRedLed();
    Delay(1);
    ResetRedLed();
    
    float RemoteOutputMessage = DisplayGetSpeedRef();
    send((uint8_t*)&RemoteOutputMessage, 4);
    waitPacketSent();
    
    waitAvailableTimeout(10);
  }
}