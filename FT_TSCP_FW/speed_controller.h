#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

#define ENCODER_INCREMENT_PER_ROTATION  4096.0
#define MILLISECS_PER_MIN               60000.0

#define INIT_WAIT_TIME          5000
#define CALIBRATION_MIN_SPEED   0.3
#define PWM_SEARCH_ROTATION     0.9

#define SPEED_CORRECTION_SIZE           128
#define SPEED_CORRECTION_FILTER_SIZE    5

#include "ft_remote_types.h"

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

void InitSpeedController(void);

SpeedControllerType GetSpeedController(void);

void SetSpeedRef(float speed_ref);

float GetSpeedRef(void);

void SpeedControllerHandler(void);

void SpeedControllerLoop(void);

uint32_t GetLinearizedIncrement(uint32_t increment_integral);

#endif