#include "battery_voltage.h"

#include "bsp_adc.h"

float filtered_battery_voltage = 0;
uint32_t battery_time = 0;
BatteryMeasureFlagType BatteryMeasureFlag = BATTERY_MEASURE_FLAG_NO_MEASURE;

void InitBatteryMeasure(void)
{
  filtered_battery_voltage = ReadBatteryVoltage();
}

void BatteryMeasureTimeHandler(void)
{
  battery_time++;
  
  if (battery_time % BATTERY_MEASURE_CYCLE == 0)
    BatteryMeasureFlag = BATTERY_MEASURE_FLAG_MEASURE;
}

void BatteryMeasureHandler(void)
{
  if (BatteryMeasureFlag == BATTERY_MEASURE_FLAG_MEASURE)
  {
    filtered_battery_voltage = (1 - BATTERY_FILTER_WEIGHT) * filtered_battery_voltage;
    filtered_battery_voltage += BATTERY_FILTER_WEIGHT * ReadBatteryVoltage();
    
    BatteryMeasureFlag = BATTERY_MEASURE_FLAG_NO_MEASURE;
  }
}

float ReadBatteryVoltage(void)
{ 
  uint32_t adc_buffer = readADC_VREF();
  float v_lsb = 2.507 / adc_buffer * 2.0162;
  
  adc_buffer = readADC_VBAT();  
  float voltage = adc_buffer * v_lsb;
  
  return voltage;
}

float GetFilteredBatteryVoltage(void)
{
  return filtered_battery_voltage;
}
