#ifndef BSP_DISPLAY_H
#define BSP_DISPLAY_H

#include "stm8l15x_conf.h"

#define LCD_DATA_PORT GPIOD
#define LCD_DATA_PORT GPIOD
#define LCD_CONTROL_PORT GPIOC
#define LCD_BACKLIGHT_PORT GPIOD
    
#define CGRAM_address_start     0x40

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


#endif