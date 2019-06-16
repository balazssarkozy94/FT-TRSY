#include "communication_handler.h"

#include "bsp_led.h"
#include "bsp_rf_com.h"
#include "bsp_timing_delay.h"

#include "speed_controller.h"
#include "ft_remote_types.h"

#include <stdio.h>

void CommunicationHandler(void)
{
  //readADC_VBAT();
  //readADC_VREF();
  
  TelescopeMessageType TelescopeOutputMessage;
  
  TelescopeOutputMessage.SpeedControllerData = GetSpeedController();
  TelescopeOutputMessage.battery_voltage = 1.231;
  
  send((uint8_t*)&TelescopeOutputMessage, sizeof(TelescopeMessageType));
  waitPacketSent();

  waitAvailableTimeout(2000);
  
  float RemoteInputMessage;
  uint8_t len;

  if(recv((uint8_t*)&RemoteInputMessage, &len))
  {
    ToggleRedLed();
    SetSpeedRef(RemoteInputMessage);
  }
}