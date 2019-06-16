#ifndef BATTERY_VOLTAGE_H
#define BATTERY_VOLTAGE_H

#define BATTERY_MEASURE_CYCLE   1000
#define BATTERY_FILTER_WEIGHT   0.2

typedef enum BatteryMeasureFlagType
{
  BATTERY_MEASURE_FLAG_NO_MEASURE,
  BATTERY_MEASURE_FLAG_MEASURE
} BatteryMeasureFlagType;

void InitBatteryMeasure(void);

void BatteryMeasureTimeHandler(void);

void BatteryMeasureHandler(void);

float ReadBatteryVoltage(void);

float GetFilteredBatteryVoltage(void);

#endif