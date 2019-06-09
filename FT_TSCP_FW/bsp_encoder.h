#ifndef BSP_ENCODER_H
#define BSP_ENCODER_H

#include "stm8l15x_conf.h"

void InitEncoder(void);

uint16_t GetEncoderValue(void);

#endif