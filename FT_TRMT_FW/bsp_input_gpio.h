#ifndef BSP_INPUT_GPIO_H
#define BSP_INPUT_GPIO_H

#define INPUT_FILTER_MS         200

typedef enum ButtonType
{
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_OK
} ButtonType;

void InitInputGpio(void);

void ButtonInterruptHandler(ButtonType button);

void ButtonFilterTimeHandler(void);

#endif