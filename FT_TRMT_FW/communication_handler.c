#include "communication_handler.h"

#include "bsp_led.h"
#include "bsp_timing_delay.h"
#include "bsp_display.h"
#include "bsp_rf_com.h"
#include "bsp_adc.h"
#include "bsp_pwm_timer.h"

#include "ft_remote_types.h"

#include <stdio.h>

char buffer [16]; 
uint32_t ADC_BUFFER;
double v_lsb;
  
void CommunicationHandler(void)
{  
  /*  
  //LCD_LOCATE(1, 1);
  ADC_BUFFER = readADC_VREF();
  v_lsb = 2.507/ADC_BUFFER*2.0162; //vref + voltage divider
  //sprintf(buffer, "VREF: 0x%03X\n", ADC_BUFFER);
  //LCD_printstring(buffer);
    
  LCD_LOCATE(1, 1);
  ADC_BUFFER = readADC_VBAT();
  sprintf(buffer, "VBAT: %.3fV   \n", (ADC_BUFFER*v_lsb));
  LCD_printstring(buffer);*/
      
  SpeedControllerType InputData;
  uint8_t len = sizeof(SpeedControllerType);
  
  //waitAvailable();
  //waitAvailableTimeout(2000);
  
  if(recv((uint8_t*)&InputData, &len))
  {
    uint8_t buf[RF22_MAX_MESSAGE_LEN] = "Poop";
    send(buf, sizeof(buf));
    
    LCD_LOCATE(1, 1);
    sprintf(buffer, "%.3f  %d \n", InputData.controller_speed_ref, (int)InputData.motor_pwm);
    LCD_printstring(buffer);
    
    LCD_LOCATE(2, 1);
    sprintf(buffer, "%.3f  %.3f \n", InputData.controller_speed_ref_with_offset, InputData.motor_speed_1sec);
    LCD_printstring(buffer);
    
    LCD_LOCATE(3, 1);
    sprintf(buffer, "%.3f  %.3f \n", InputData.motor_speed_half_turn, InputData.motor_speed_whole_turn);
    LCD_printstring(buffer);
    
    LCD_LOCATE(4, 1);
    sprintf(buffer, "%.3f  %.3f \n", InputData.expected_pos, InputData.actual_pos);
    LCD_printstring(buffer);
  }
}