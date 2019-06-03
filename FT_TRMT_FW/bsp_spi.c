#include "bsp_spi.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_it.h"

void InitSpi(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, ENABLE);
  SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_16, SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, (uint8_t)0x00);
  SPI_Cmd(SPI1, ENABLE);
}