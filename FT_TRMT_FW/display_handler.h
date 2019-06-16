#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include "ft_remote_types.h"

#define COMMUNICATION_COM_TIMEOUT       4000
#define WELCOME_SCREEN_TIME             5000
#define CALIBRATION_DONE_TIME           4000

#define LCD_LINE_BUFFER_SIZE    17

typedef enum DisplayStateType
{
  DISPLAY_STATE_WELCOME,
  DISPLAY_STATE_START_CALIBRATION,
  DISPLAY_STATE_CALIBRATION_IN_PROGRESS,
  DISPLAY_STATE_CALIBRATION_DONE,
  DISPLAY_STATE_DIAGNOSTICS,
  DISPLAY_STATE_SET_SPEED
} DisplayStateType;

typedef enum DisplayDiagPageType
{
  DISPLAY_DIAG_PAGE_POSITION,
  DISPLAY_DIAG_PAGE_SPEED,
  DISPLAY_DIAG_PAGE_MOTOR,
  DISPLAY_DIAG_PAGE_BATTERIES
} DisplayDiagPageType;

typedef enum DisplayComTimeoutType
{
  DISPLAY_COM_TIMEOUT,
  DISPLAY_COM_NO_TIMEOUT
} DisplayComTimeoutType;

typedef enum DisplayClearedStateType
{
  DISPLAY_NOT_CLEARED,
  DISPLAY_CLEARED
} DisplayClearedStateType;

void DisplayHandler(void);

void DisplayTimeHandler(void);

void DisplayMessageArrived(TelescopeMessageType* InputData);

void DisplaySwitchState(DisplayStateType NewState);

void DisplaySwitchDiagPage(DisplayDiagPageType NewPage);

void DisplaySwitchComTimeoutState(DisplayComTimeoutType NewTimeoutState);

void DisplayLeftButtonPressed(void);

void DisplayRightButtonPressed(void);

void DisplayUpButtonPressed(void);

void DisplayDownButtonPressed(void);

void DisplayOkButtonPressed(void);

void DisplayStateMachineHandler(void);

float DisplayGetSpeedRef(void);

#endif