#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

#define ENCODER_INCREMENT_PER_ROTATION  4096.0
#define MILLISECS_PER_MIN               60000.0

#define INIT_WAIT_TIME          5000
#define CALIBRATION_MIN_SPEED   0.3
#define PWM_SEARCH_ROTATION     0.9

#define SPEED_CORRECTION_SIZE           128
#define SPEED_CORRECTION_FILTER_SIZE    5

#define CONTROLLER_KP           30
#define CONTROLLER_KI           500
#define CORRECTION_MIN          0.2
#define POS_ERROR_KP            0.5
#define SPEED_REF_COR_DIV       3

#define SPEED_1S_FILTER_WEIGHT  0.2

#include "ft_remote_types.h"

void InitSpeedController(void);

SpeedControllerType GetSpeedController(void);

void SetSpeedRef(float speed_ref);

float GetSpeedRef(void);

void SpeedControllerHandler(void);

void SpeedControllerLoop(void);

uint32_t GetLinearizedIncrement(uint32_t increment_integral);

#endif