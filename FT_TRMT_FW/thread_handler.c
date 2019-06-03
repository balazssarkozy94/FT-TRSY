#include "thread_handler.h"

#include "bsp_led.h"
#include "bsp_timing_delay.h"
#include "bsp_display.h"
#include "bsp_rf_com.h"
#include "bsp_adc.h"
#include "bsp_pwm_timer.h"

#include <stdio.h>

void MilliThread(void)
{
  ToggleRedLed();
}

void LoopThread(void)
{
  char buffer [16]; 
  uint32_t ADC_BUFFER;
  double v_lsb;
  
  SetPwm(500);
    
  LCD_LOCATE(1, 1);
  ADC_BUFFER = readADC_VREF();
  v_lsb = 2.507/ADC_BUFFER*2.0162; //vref + voltage divider
  sprintf(buffer, "VREF: 0x%03X\n", ADC_BUFFER);
  LCD_printstring(buffer);
    
  LCD_LOCATE(2, 1);
  ADC_BUFFER = readADC_VBAT();
  sprintf(buffer, "VBAT: %.3fV\n", (ADC_BUFFER*v_lsb));
  LCD_printstring(buffer);
    
  ToggleGreenLed();
      
  uint8_t data[RF22_MAX_MESSAGE_LEN] = "none";
  uint8_t len = sizeof(data);
  //waitAvailable();
    
  waitAvailableTimeout(2000);
  if(recv(data, &len))
  {
    //ToggleRedLed();
  }
    
  LCD_LOCATE(3, 1);
  sprintf(buffer, "%s\n", (char*)(data));
  LCD_printstring(buffer);
    
  /*
  uint8_t buf[RF22_MAX_MESSAGE_LEN] = "Poop";
  send(buf, sizeof(buf));
  waitPacketSent();
  */
}