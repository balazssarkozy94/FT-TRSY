#include "communication_handler.h"

#include "bsp_led.h"
#include "bsp_timing_delay.h"
#include "bsp_rf_com.h"
#include "bsp_adc.h"
#include "bsp_pwm_timer.h"
#include "bsp_encoder.h"

#include "speed_controller.h"

#include <stdio.h>

void CommunicationHandler(void)
{
  //readADC_VBAT();
  //readADC_VREF();
  
  SetGreenLed();
  
  SpeedControllerType SpeedControllerData = GetSpeedController();
  
  send((uint8_t*)&SpeedControllerData, sizeof(SpeedControllerType));
  waitPacketSent();
  
  waitAvailableTimeout(200);
  
  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  
  if(recv(buf, &len))
  {
    ToggleRedLed();
  }
  
  ResetGreenLed();
  
  //ShutdownRFM22B();
  
  Delay(1000);
  
  //RfComInit();
  
  //EnableRFM22B();
  
  //EXTI_ClearITPendingBit(EXTI_IT_Pin3);
  //enableInterrupts();
  Delay(100);
  
  //RF22init();
  Delay(200);
}