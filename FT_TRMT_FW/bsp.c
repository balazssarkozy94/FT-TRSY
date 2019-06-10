#include "bsp.h"

#include "bsp_itc_config.h"
#include "bsp_timing_delay.h"
#include "bsp_clk_config.h"
#include "bsp_input_gpio.h"
#include "bsp_led.h"
#include "bsp_display.h"
#include "bsp_rf_com.h"
#include "bsp_pwm_timer.h"
#include "bsp_spi.h"
#include "bsp_adc.h"

void InitHardware(void)
{
  CLK_Config();
    
  TimingDelay_Init();  
  
  InitInputGpio();
  InitLed();    
  InitPwmTimer();
  InitSpi();
  InitAdc();
  
  ITC_Config();
  enableInterrupts();
  
  RfComInit();  
  LCD_INIT();
}