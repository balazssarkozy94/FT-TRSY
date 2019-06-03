#include "bsp_adc.h"

#include "bsp_timing_delay.h"

void InitAdc(void)
{
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast);
  
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