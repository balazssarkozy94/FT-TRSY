#ifndef EEPROM_H
#define EEPROM_H

#include "stm8l15x_flash.h"

void WriteEepromByte(uint8_t dataByte, uint16_t address);

uint8_t ReadEepromByte(uint16_t address);

#endif