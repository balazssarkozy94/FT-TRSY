#include "bsp_input_gpio.h"

#include "stm8l15x.h"
#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_it.h"

#include "bsp_led.h"

#include "display_handler.h"

uint32_t filter_time = 0;

void InitInputGpio(void)
{
  GPIO_Init(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_PU_IT);
  GPIO_Init(GPIOC, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_PU_IT);
  
  EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Falling);
}

void ButtonInterruptHandler(ButtonType button)
{
  if (filter_time < INPUT_FILTER_MS)
    return;
  
  filter_time = 0;
  
  switch (button)
  {
  case BUTTON_LEFT:
    DisplayLeftButtonPressed();
    break;
    
  case BUTTON_RIGHT:
    DisplayRightButtonPressed();
    break;
  
  case BUTTON_UP:
    DisplayUpButtonPressed();
    break;
    
  case BUTTON_DOWN:
    DisplayDownButtonPressed();
    break;
      
  case BUTTON_OK:
    DisplayOkButtonPressed();
    break;
  default:
    break;
  }
}

void ButtonFilterTimeHandler(void)
{
  if (filter_time < INPUT_FILTER_MS)
      filter_time++;
}