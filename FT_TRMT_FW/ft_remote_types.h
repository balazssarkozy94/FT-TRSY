#ifndef FT_REMOTE_TYPES
#define FT_REMOTE_TYPES

#include "stm8l15x_conf.h"

typedef enum SpeedControllerStateType
{
  SPEED_CONTROLLER_NOT_INITED,
  SPEED_CONTROLLER_INITED,
  SPEED_CONTROLLER_CALIBRATION_SEARCH_PWM,
  SPEED_CONTROLLER_CALIBRATION_START,
  SPEED_CONTROLLER_CALIBRATION_IN_PROGRESS,
  SPEED_CONTROLLER_CALIBRATION_FINISHED,
  SPEED_CONTROLLER_OK
} SpeedControllerStateType;

typedef struct SpeedControllerType
{
  float motor_speed_1sec;
  float motor_speed_1sec_filtered;
  float motor_speed_whole_turn;
  float position_error_whole_turn;
  float controller_speed_ref;
  float controller_speed_ref_with_offset;
  float expected_pos;
  float actual_pos;
  int32_t motor_pwm;
  SpeedControllerStateType SpeedControllerState;
} SpeedControllerType;

typedef struct TelescopeMessageType
{
  SpeedControllerType SpeedControllerData;
  float battery_voltage;
} TelescopeMessageType;

#endif