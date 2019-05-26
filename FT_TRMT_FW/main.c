#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "timing_delay.h"
#include "HD44780.h"
#include "RFM22B.h"
#include "stm8l15x_it.h"
#include <stdio.h>

#include "bsp.h"

  char buffer [16]; 
  uint32_t ADC_BUFFER;
  double v_lsb;

int main( void )
{  

  InitHardware();
  
  while(1)
  {
    Delay(500);
    
    LCD_LOCATE(1, 1);
    ADC_BUFFER = readADC_VREF();
    v_lsb = 2.507/ADC_BUFFER*2.0162; //vref + voltage divider
    sprintf(buffer, "VREF: 0x%03X\n", ADC_BUFFER);
    LCD_printstring(buffer);
    
    LCD_LOCATE(2, 1);
    ADC_BUFFER = readADC_VBAT();
    sprintf(buffer, "VBAT: %.3fV\n", (ADC_BUFFER*v_lsb));
    LCD_printstring(buffer);
    
    LCD_LOCATE(4, 1);
    sprintf(buffer, "millis: %d  \n", (int)millis());
    LCD_printstring(buffer);
    
    GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
      
    uint8_t data[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(data);
    //waitAvailable();
    waitAvailableTimeout(2000);
    if(recv(data, &len))
    {
       GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
    }
    
    /*uint8_t buf[RF22_MAX_MESSAGE_LEN] = "Poop";
    send(buf, sizeof(buf));
    waitPacketSent();*/
  }
}

