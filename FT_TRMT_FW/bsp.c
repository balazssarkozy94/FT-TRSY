#include "bsp.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "timing_delay.h"
#include "HD44780.h"
#include "RFM22B.h"
#include "stm8l15x_it.h"

void InitHardware(void)
{
  CLK_Config();
  GPIO_Config();
  ShutdownRFM22B();
  TimingDelay_Init();
  ITC_Config();
  TIM1_Config();
  ADC_Config();
  SPI_Config();
  EnableRFM22B();
  
  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
  enableInterrupts();
  Delay(100);
  
  if(!RF22init())
  {
    while(1)
    {
      Delay(500);
      GPIO_ToggleBits(GPIOC, GPIO_Pin_0);
    }
  }
  
  /* Min. delay to wait before initialization after LCD power ON */
  /* Value is ms units*/
  Delay(100);
  /* Initialization of LCD */
  LCD_INIT();
  /* LCD Clear Display */
  LCD_CLEAR_DISPLAY();
  /* Set @CGRAM address start */
  LCD_CMD(CGRAM_address_start);
}

void CLK_Config(void)
{
  /* Enable HSE */
  CLK_HSEConfig(CLK_HSE_ON);
  
  /* Wait for HSE to be ready */
  while(CLK_GetFlagStatus(CLK_FLAG_HSERDY) == RESET);
  
  /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE);
}

void GPIO_Config(void)
{
  /* Configure push button inputs for interrupt */
  GPIO_Init(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_PU_IT);
  GPIO_Init(GPIOC, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_PU_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger_Falling);
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Falling);
  
  /* Configure LED outputs */
  GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast);
  
  /* Configure LCD pins */
  GPIO_Init(GPIOD, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOC, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast);
  
  /* Configure RFM22B pins */
  GPIO_Init(GPIOB, GPIO_Pin_2 | GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_In_PU_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
  
  /* Configure VBAT_MEAS_EN pin */
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast);

}

void ITC_Config(void)
{
  ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_IRQn, ITC_PriorityLevel_1);
  ITC_SetSoftwarePriority(EXTI3_IRQn, ITC_PriorityLevel_3);
  ITC_SetSoftwarePriority(EXTI4_IRQn, ITC_PriorityLevel_3);
  ITC_SetSoftwarePriority(EXTI5_IRQn, ITC_PriorityLevel_3);
  ITC_SetSoftwarePriority(EXTI6_IRQn, ITC_PriorityLevel_3);
}

void TIM1_Config(void)
{
    /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  
    /* Time Base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);
  
  TIM1_OC3Init(TIM1_OCMode_PWM1, TIM1_OutputState_Disable, TIM1_OutputNState_Enable, 25, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);
  TIM1_OC3PreloadConfig(DISABLE);
  TIM1_ARRPreloadConfig(ENABLE);
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM1_Cmd(ENABLE);
}

void SPI_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);
  SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_16, SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, (uint8_t)0x00);
//SPI_ITConfig()
  SPI_Cmd(SPI1, ENABLE);
}

void ADC_Config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ChannelCmd(ADC1, ADC_Channel_19, ENABLE); //IN19, VBAT
  ADC_SoftwareStartConv(ADC1);
  
}

uint16_t readADC_VBAT(void)
{
  uint8_t i = 0;
  uint32_t accumulator = 0;
  ADC_ChannelCmd(ADC1, ADC_Channel_20, DISABLE);        //VREF OFF
  ADC_ChannelCmd(ADC1, ADC_Channel_19, ENABLE);         //VBAT ON
  ADC_SoftwareStartConv(ADC1);
  GPIO_SetBits(GPIOB, GPIO_Pin_0);
  Delay(1);
  for(i = 0; i<ADC_BUFFER_SIZE; i++)
  {
    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
    accumulator += ADC_GetConversionValue(ADC1);
  }

  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
  return (accumulator/ADC_BUFFER_SIZE);
}

uint16_t readADC_VREF(void)
{
  uint8_t i = 0;
  uint32_t accumulator = 0;
  ADC_ChannelCmd(ADC1, ADC_Channel_19, DISABLE);        //VBAT OFF
  ADC_ChannelCmd(ADC1, ADC_Channel_20, ENABLE);         //VREF ON
  ADC_SoftwareStartConv(ADC1);
  GPIO_SetBits(GPIOB, GPIO_Pin_0);
  Delay(1);
  for(i = 0; i<ADC_BUFFER_SIZE; i++)
  {
    ADC_SoftwareStartConv(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
    accumulator += ADC_GetConversionValue(ADC1);
  }
  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
  return (accumulator/ADC_BUFFER_SIZE);
}