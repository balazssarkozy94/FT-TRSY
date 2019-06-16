#include "communication_handler.h"

#include "bsp_led.h"
#include "bsp_rf_com.h"
#include "bsp_timing_delay.h"

#include "speed_controller.h"
#include "battery_voltage.h"
#include "ft_remote_types.h"

void CommunicationHandler(void)
{  
  TelescopeMessageType TelescopeOutputMessage;
  
  TelescopeOutputMessage.SpeedControllerData = GetSpeedController();
  TelescopeOutputMessage.battery_voltage = GetFilteredBatteryVoltage();;
  
  send((uint8_t*)&TelescopeOutputMessage, sizeof(TelescopeMessageType));
  waitPacketSent();

  waitAvailableTimeout(500);
  
  float RemoteInputMessage;
  uint8_t len;

  if(recv((uint8_t*)&RemoteInputMessage, &len))
  {
    SetSpeedRef(RemoteInputMessage);
    
    SetRedLed();
    Delay(1);
    ResetRedLed();
  }
}