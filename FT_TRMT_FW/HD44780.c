#include "stm8l15x_conf.h"
#include "stm8l15x_gpio.h"
#include "timing_delay.h"
#include "stm8l15x_it.h"
#include "HD44780.h"

/**
  * @brief  Activate Enable Pin from LCD module
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_ENABLE (void)
{
  GPIO_SetBits(GPIOC, GPIO_Pin_4);
  Delay(2);
  GPIO_ResetBits(GPIOC, GPIO_Pin_4);
}

/**
  * @brief  Command data sent to LCD module
  * @param  command value to be sent
  * @param  None
  * @retval None
  */
void LCD_CMD(unsigned char cmd_data)
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  GPIO_WriteBit(GPIOD, GPIO_Pin_7, (cmd_data >> 7) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_6, (cmd_data >> 6) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_5, (cmd_data >> 5) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_4, (cmd_data >> 4) & 0x01 ? SET : RESET);
  LCD_ENABLE();
  GPIO_WriteBit(GPIOD, GPIO_Pin_7, (cmd_data >> 3) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_6, (cmd_data >> 2) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_5, (cmd_data >> 1) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_4, (cmd_data >> 0) & 0x01 ? SET : RESET);
  LCD_ENABLE();
  Delay(2);
}


/**
  * @brief  Initializes HD44780 LCD module in 4-bit mode
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_INIT(void)
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_3);
  GPIO_SetBits(GPIOC, GPIO_Pin_4);
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  //Initialization of HD44780-based LCD (4-bit HW)
  LCD_CMD(0x33);
  Delay(4);
  LCD_CMD(0x32);
  Delay(4);
  //Function Set 4-bit mode
  LCD_CMD(0x28);
  //Display On/Off Control
  LCD_CMD(0x0C);
  //Entry mode set
  LCD_CMD(0x06);
  LCD_CLEAR_DISPLAY();
  //Minimum delay to wait before driving LCD module
  Delay(200);
}

/**
  * @brief  Print Character on LCD module
  * @param  Ascii value of character
  * @param  None
  * @retval None
  */
void LCD_printchar(char ascode)
{
  GPIO_SetBits(GPIOC, GPIO_Pin_2);
  GPIO_WriteBit(GPIOD, GPIO_Pin_7, (ascode >> 7) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_6, (ascode >> 6) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_5, (ascode >> 5) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_4, (ascode >> 4) & 0x01 ? SET : RESET);
  LCD_ENABLE();
  GPIO_WriteBit(GPIOD, GPIO_Pin_7, (ascode >> 3) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_6, (ascode >> 2) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_5, (ascode >> 1) & 0x01 ? SET : RESET);
  GPIO_WriteBit(GPIOD, GPIO_Pin_4, (ascode >> 0) & 0x01 ? SET : RESET);
  LCD_ENABLE();
  Delay(2);
}

/**
  * @brief  Display of a characters string
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
void LCD_printstring(char *text)
{
  do
  {
    LCD_printchar(*text++);
  }
  while (*text != '\n');
}

/**
  * @brief  Clear LCD module display
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_CLEAR_DISPLAY(void)
{
  LCD_CMD(0x01);
  Delay(2);
}

/**
  * @brief  Set Cursor on second row 1st digit
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_2ndROW(void)
{
  LCD_CMD(0xC0);
}

/**
  * @brief  Set Cursor to Home position
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_HOME(void)
{
  LCD_CMD(0x02);
  Delay(2);
}

/**
  * @brief  Shift display to left
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_LSHIFT(void)
{
  LCD_CMD(0x18);
}

/**
  * @brief  Shift display to right
  * @param  Text to be displayed
  * @param  None
  * @retval None
  */
void LCD_RSHIFT(void)
{
  LCD_CMD(0x1C);
}

/**
  * @brief  Set Display On
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_DISP_ON(void)
{
  LCD_CMD(0x0C);
}

/**
  * @brief  Set Display Off
  * @param  None
  * @param  None
  * @retval None
  */
void LCD_DISP_OFF(void)
{
  LCD_CMD(0x08);
}

/**
  * @brief  Set Cursor to a specified location given by row and column information
  * @param  Row Number (1 to 4)
  * @param  Column Number (1 to 16) Assuming a 4 X 16 characters display
  * @retval None
  */
void LCD_LOCATE(u8 row, u8 column)
{
  column--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      LCD_CMD(column |= 0x80);
      break;
    case 2:
      /* Set cursor to 2nd row address and add index*/
      LCD_CMD(column |= 0x40 | 0x80);
      break;
    case 3:
      /* Set cursor to 3rd row address and add index*/
      LCD_CMD(column |= 0x10 | 0x80);
      break;
    case 4:
      /* Set cursor to 4th row address and add index*/
      LCD_CMD(column |= 0x50 | 0x80);
      break;
    default:
      break;
  }
}