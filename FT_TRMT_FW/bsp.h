#ifndef BSP_H
#define BSP_H

#define TIM1_PERIOD             50
#define TIM1_PRESCALER          1
#define TIM1_REPTETION_COUNTER  0

//MAX BUFFER SIZE: 255
#define ADC_BUFFER_SIZE         100

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "timing_delay.h"
#include "HD44780.h"
#include "RFM22B.h"
#include "stm8l15x_it.h"

void InitHardware(void);

void CLK_Config(void);

void GPIO_Config(void);

void ITC_Config(void);

void TIM1_Config(void);

void SPI_Config(void);

void ADC_Config(void);

uint16_t readADC_VBAT(void);

uint16_t readADC_VREF(void);

#endif