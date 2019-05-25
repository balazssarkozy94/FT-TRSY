#include "eeprom.h"
#include "stm8l15x_flash.h"

void WriteEepromByte(uint8_t dataByte, uint16_t address)
{
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
  FLASH_Unlock(FLASH_MemType_Data);
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
  
  FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + address, dataByte);
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);
  
  FLASH_Lock(FLASH_MemType_Data);
}

uint8_t ReadEepromByte(uint16_t address)
{
  return FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + address);
}