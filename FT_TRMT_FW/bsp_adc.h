#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stm8l15x_conf.h"

#define ADC_BUFFER_SIZE         100

void InitAdc(void);

uint16_t readADC_VBAT(void);

uint16_t readADC_VREF(void);

#endif