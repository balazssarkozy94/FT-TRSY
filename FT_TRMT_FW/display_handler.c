#include "display_handler.h"

#include "bsp_display.h"
#include "display_backlight.h"

#include "ft_remote_types.h"
#include <stdio.h>

DisplayStateType DisplayState = DISPLAY_STATE_WELCOME;
DisplayComTimeoutType DisplayComTimeout = DISPLAY_COM_TIMEOUT;
DisplayClearedStateType DisplayClearedState = DISPLAY_NOT_CLEARED;

char LcdLineBuffer[LCD_LINE_BUFFER_SIZE];
uint32_t display_com_timeout = 0;
uint32_t welcome_screen_timeout = 0;

TelescopeMessageType TelescopeData;

float display_speed_ref = -2;

void DisplayHandler(void)
{
  DisplayStateMachineHandler();
  BrightnessHandler();
}

void DisplayTimeHandler(void)
{
  display_com_timeout++;
  welcome_screen_timeout++;
  
  if (display_com_timeout >= COMMUNICATION_COM_TIMEOUT)
    DisplaySwitchComTimeoutState(DISPLAY_COM_TIMEOUT);
  else
    DisplaySwitchComTimeoutState(DISPLAY_COM_NO_TIMEOUT);
}

void DisplayMessageArrived(TelescopeMessageType* InputData)
{
  TelescopeData = *InputData;
  display_com_timeout = 0;
}

void DisplaySwitchState(DisplayStateType NewState)
{
  DisplayState = NewState;
  DisplayClearedState = DISPLAY_NOT_CLEARED;
}

void DisplaySwitchComTimeoutState(DisplayComTimeoutType NewTimeoutState)
{
  if (DisplayComTimeout != NewTimeoutState)
  {
    DisplayComTimeout = NewTimeoutState;
    DisplayClearedState = DISPLAY_NOT_CLEARED;
  }
}

void DisplayLeftButtonPressed(void)
{
  // TODO: kaki
  DisplaySwitchState(DISPLAY_STATE_DIAGNOSTICS);
}

void DisplayRightButtonPressed(void)
{
  
}

void DisplayUpButtonPressed(void)
{
  if (DisplayComTimeout == DISPLAY_COM_TIMEOUT || DisplayState != DISPLAY_STATE_SET_SPEED)
    IncreaseBrightness();
}

void DisplayDownButtonPressed(void)
{
  if (DisplayComTimeout == DISPLAY_COM_TIMEOUT || DisplayState != DISPLAY_STATE_SET_SPEED)
    DecreaseBrightness();
}

void DisplayOkButtonPressed(void)
{
  if (DisplayComTimeout != DISPLAY_COM_TIMEOUT)
  {
    if (DisplayState == DISPLAY_STATE_DIAGNOSTICS)
      DisplaySwitchState(DISPLAY_STATE_SET_SPEED);
    else if (DisplayState == DISPLAY_STATE_SET_SPEED)
      DisplaySwitchState(DISPLAY_STATE_DIAGNOSTICS);
    else if (DisplayState == DISPLAY_STATE_START_CALIBRATION)
      display_speed_ref = -1;
  }
}

void DisplayStateMachineHandler(void)
{    
  if (DisplayClearedState == DISPLAY_NOT_CLEARED)
  {
    LCD_CLEAR_DISPLAY();
    DisplayClearedState = DISPLAY_CLEARED;
  }
  
  if (DisplayComTimeout == DISPLAY_COM_TIMEOUT)
  {
    if (DisplayState == DISPLAY_STATE_START_CALIBRATION || DisplayState == DISPLAY_STATE_WELCOME)
    {
      LCD_LOCATE(2, 3);
      sprintf(LcdLineBuffer, "No telescope\n");
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(3, 5);
      sprintf(LcdLineBuffer, "detected\n");
      LCD_printstring(LcdLineBuffer);
    }      
    else
    {
      LCD_LOCATE(2, 1);
      sprintf(LcdLineBuffer, "Connection lost\n");
      LCD_printstring(LcdLineBuffer);
    
      LCD_LOCATE(3, 1);
      sprintf(LcdLineBuffer, "timeout: %d\n", (int)((display_com_timeout - COMMUNICATION_COM_TIMEOUT) / 1000));
      LCD_printstring(LcdLineBuffer);
    }
  }
  else if (DisplayState == DISPLAY_STATE_WELCOME)
  {
    LCD_LOCATE(2, 5);
    sprintf(LcdLineBuffer, "Welcome!\n");
    LCD_printstring(LcdLineBuffer);  
    
    if (welcome_screen_timeout > WELCOME_SCREEN_TIME)
    {
      if (TelescopeData.SpeedControllerData.SpeedControllerState != SPEED_CONTROLLER_OK)
      {
        DisplaySwitchState(DISPLAY_STATE_START_CALIBRATION);
      }
      else
      {
        DisplaySwitchState(DISPLAY_STATE_DIAGNOSTICS);
      }
    }
  }
  else if (DisplayState == DISPLAY_STATE_START_CALIBRATION)
  {
    if (TelescopeData.SpeedControllerData.SpeedControllerState == SPEED_CONTROLLER_NOT_INITED)
    {
      LCD_LOCATE(2, 3);
      sprintf(LcdLineBuffer, "Press OK to\n");
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(3, 3);
      sprintf(LcdLineBuffer, "calibrate!\n");
      LCD_printstring(LcdLineBuffer);
    }
    else
    {
      DisplaySwitchState(DISPLAY_STATE_CALIBRATION_IN_PROGRESS);
    }
  }
  else if (DisplayState == DISPLAY_STATE_CALIBRATION_IN_PROGRESS)
  {
    if (TelescopeData.SpeedControllerData.SpeedControllerState != SPEED_CONTROLLER_OK)
    {
      int stage_1_state;
      int stage_2_state;
            
      if (TelescopeData.SpeedControllerData.SpeedControllerState < SPEED_CONTROLLER_CALIBRATION_IN_PROGRESS)
      {
        stage_1_state = (int)(100 * TelescopeData.SpeedControllerData.actual_pos);
        stage_2_state = 0;
      }
      else
      {
        stage_1_state = 100;
        stage_2_state = (int)(100 * TelescopeData.SpeedControllerData.actual_pos);
      }      
      
      LCD_LOCATE(1, 1);
      sprintf(LcdLineBuffer, "Calibration\n");
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(2, 2);
      sprintf(LcdLineBuffer, "Stage1: %3d%\n", stage_1_state);
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(3, 2);
      sprintf(LcdLineBuffer, "Stage2: %3d%\n", stage_2_state);
      LCD_printstring(LcdLineBuffer);
      
      if (TelescopeData.SpeedControllerData.SpeedControllerState == SPEED_CONTROLLER_NOT_INITED)
      {
        DisplaySwitchState(DISPLAY_STATE_START_CALIBRATION);
      }
    }
    else
    {
      DisplaySwitchState(DISPLAY_STATE_DIAGNOSTICS);
    }
    
    display_speed_ref = -2;
  }
  else if (DisplayState == DISPLAY_STATE_DIAGNOSTICS)
  {    
    if (TelescopeData.SpeedControllerData.SpeedControllerState != SPEED_CONTROLLER_OK)
    {
      DisplaySwitchState(DISPLAY_STATE_START_CALIBRATION);
      return;
    }
    
    LCD_LOCATE(1, 1);
    sprintf(LcdLineBuffer, "%.3f  %d \n", TelescopeData.SpeedControllerData.controller_speed_ref, (int)TelescopeData.SpeedControllerData.motor_pwm);
    LCD_printstring(LcdLineBuffer);
      
    LCD_LOCATE(2, 1);
    sprintf(LcdLineBuffer, "%.3f  %.3f \n", TelescopeData.SpeedControllerData.controller_speed_ref_with_offset, TelescopeData.SpeedControllerData.motor_speed_1sec_filtered);
    LCD_printstring(LcdLineBuffer);
      
    LCD_LOCATE(3, 1);
    sprintf(LcdLineBuffer, "%.3f  %.3f \n", TelescopeData.SpeedControllerData.motor_speed_whole_turn, TelescopeData.SpeedControllerData.position_error_whole_turn);
    LCD_printstring(LcdLineBuffer);
      
    LCD_LOCATE(4, 1);
    sprintf(LcdLineBuffer, "%.3f  %.3f \n", TelescopeData.SpeedControllerData.expected_pos, TelescopeData.SpeedControllerData.actual_pos);
    LCD_printstring(LcdLineBuffer);
    
    /*uint32_t ADC_BUFFER;
    double v_lsb;
    
    //LCD_LOCATE(1, 1);
    ADC_BUFFER = readADC_VREF();
    v_lsb = 2.507/ADC_BUFFER*2.0162; //vref + voltage divider
    //sprintf(buffer, "VREF: 0x%03X\n", ADC_BUFFER);
    //LCD_printstring(buffer);
      
    LCD_LOCATE(1, 1);
    ADC_BUFFER = readADC_VBAT();
    sprintf(buffer, "VBAT: %.3fV   \n", (ADC_BUFFER*v_lsb));
    LCD_printstring(buffer);*/
  }
  else if (DisplayState == DISPLAY_STATE_SET_SPEED)
  {
    if (TelescopeData.SpeedControllerData.SpeedControllerState != SPEED_CONTROLLER_OK)
    {
      DisplaySwitchState(DISPLAY_STATE_START_CALIBRATION);
      return;
    }
    
    LCD_LOCATE(1, 1);
    sprintf(LcdLineBuffer, "Speed settings\n");
    LCD_printstring(LcdLineBuffer);
    
    LCD_LOCATE(2, 1);
    sprintf(LcdLineBuffer, "actual: %.3f \n", TelescopeData.SpeedControllerData.controller_speed_ref);
    LCD_printstring(LcdLineBuffer);
    
    LCD_LOCATE(3, 1);
    sprintf(LcdLineBuffer, "new:    %.3f \n", 1.234);
    LCD_printstring(LcdLineBuffer);
  }
}

float DisplayGetSpeedRef(void)
{
  return display_speed_ref;
}
