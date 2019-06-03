#include "thread_handler.h"

#include "bsp_led.h"
#include "bsp_timing_delay.h"
#include "bsp_rf_com.h"
#include "bsp_adc.h"
#include "bsp_pwm_timer.h"

#include <stdio.h>

int i = 0;

void MilliThread(void)
{
  
}

void LoopThread(void)
{
  /*for(delete_me = 0; delete_me < 10; delete_me++)
  {
      
  Delay(500);
  GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
  GPIO_ToggleBits(GPIOC, GPIO_Pin_1);
  readADC_VBAT();
  readADC_VREF();
    
  new_encoder = TIM3_GetCounter();
  TEST_DELETE_ME = new_encoder - last_encoder; //encoder
    
  BUFFER_FOR_ENC_TEST[delete_me] = TEST_DELETE_ME;
   
  last_encoder = new_encoder;
    
  }*/
    
  /*waitAvailable();
  if(recv(data, &len))
  {
    GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
  }*/
    
  Delay(200);
  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  sprintf(buf, "kutyafasz %d", i);
  send(buf, sizeof(buf)); 
  waitPacketSent();
  GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
  i++;
}