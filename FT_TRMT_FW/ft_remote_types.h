#ifndef FT_REMOTE_TYPES
#define FT_REMOTE_TYPES

#include "stm8l15x_conf.h"

typedef struct SpeedControllerType
{
  float motor_speed_1sec;
  float motor_speed_half_turn;
  float motor_speed_whole_turn;
  float controller_speed_ref;
  float controller_speed_ref_with_offset;
  float expected_pos;
  float actual_pos;
  int32_t motor_pwm;
} SpeedControllerType;

#endif