#include "bsp.h"
#include "timing_delay.h"
#include "HD44780.h"
#include "RFM22B.h"
#include "eeprom.h"

#include <stdio.h>

volatile int kuki[10];
volatile uint8_t rec_buf[RF22_MAX_MESSAGE_LEN];

int main( void )
{
  char buffer[17];
  
  InitHardware();
    
  while(1)
  {  
    //Delay(500);
    
    LCD_LOCATE(1, 1);    
    uint16_t ADC_BUFFER = readADC_VREF();
    float v_lsb = 2.507/ADC_BUFFER*2.0162; //vref + voltage divider
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

    /*for (int i = 0; i < 10; i++)
    {
      kuki[i] = ReadEepromByte(i);
    }*/
    
    /*uint8_t buf[RF22_MAX_MESSAGE_LEN] = "Poop";
    send(buf, sizeof(buf));
    waitPacketSent();*/
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, RESET);
    
    waitAvailable();
    //waitAvailableTimeout(2000);
    uint8_t len;
    recv((uint8_t*)rec_buf, &len);    

    GPIO_WriteBit(GPIOC, GPIO_Pin_0, SET);
  }
}

