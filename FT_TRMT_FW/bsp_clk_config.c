#include "bsp_clk_config.h"

#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"

void CLK_Config(void)
{
  /* Enable HSE */
  CLK_HSEConfig(CLK_HSE_ON);
  
  /* Wait for HSE to be ready */
  while(CLK_GetFlagStatus(CLK_FLAG_HSERDY) == RESET);
  
  /* High speed internal clock prescaler: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE);
}