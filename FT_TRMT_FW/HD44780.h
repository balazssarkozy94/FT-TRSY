/**
  ********************************************************************************
  * @file    HD44780.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16-January-2012
  * @brief   This file contains all functions prototypes and macros for the HD44780 LCD controller.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * FOR MORE INFORMATION PLEASE READ CAREFULLY THE LICENSE AGREEMENT FILE
  * LOCATED IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HD44780_H
#define __HD44780_H

#define LCD_DATA_PORT GPIOD
#define LCD_DATA_PORT GPIOD
#define LCD_CONTROL_PORT GPIOC
#define LCD_BACKLIGHT_PORT GPIOD
    
/* HD44780 CGRAM address start */
#define CGRAM_address_start 0x40

/* Exported constants --------------------------------------------------------*/
/* Exported macros ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @addtogroup HD44780_Exported_Functions
  * @{
  */
void LCD_LOAD_CGRAM(char tab[], uint8_t charnum);
void LCD_PWRON (void);
void LCD_PWROFF (void);
void LCD_INIT(void);
void LCD_printchar(char ascode);
void LCD_printstring(char *text);
void LCD_CMD(unsigned char data);
void LCD_CLEAR_DISPLAY(void);
void LCD_2ndROW(void);
void LCD_HOME(void);
void LCD_LSHIFT(void);
void LCD_RSHIFT(void);
void LCD_DISP_ON(void);
void LCD_DISP_OFF(void);
void LCD_LOCATE(uint8_t row, uint8_t column);
void LCD_printf(const char *fmt, ...);


#endif /* __HD44780_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
