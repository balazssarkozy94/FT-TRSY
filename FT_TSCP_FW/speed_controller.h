#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

#define ENCODER_INCREMENT_PER_ROTATION  4096.0
#define MILLISECS_PER_MIN               60000.0

#include "ft_remote_types.h"

void InitSpeedController(void);

SpeedControllerType GetSpeedController(void);

void SetSpeedRef(float speed_ref);

float GetSpeedRef(void);

void SpeedControllerHandler(void);

#endif