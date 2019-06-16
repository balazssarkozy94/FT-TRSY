#include "display_handler.h"

#include "bsp_display.h"
#include "display_backlight.h"

#include "ft_remote_types.h"
#include "battery_voltage.h"

#include "stdio.h"
#include "string.h"

DisplayStateType DisplayState = DISPLAY_STATE_WELCOME;
DisplayDiagPageType DisplayDiagPage = DISPLAY_DIAG_PAGE_POSITION;

DisplayComTimeoutType DisplayComTimeout = DISPLAY_COM_TIMEOUT;
DisplayClearedStateType DisplayClearedState = DISPLAY_NOT_CLEARED;

char LcdLineBuffer[LCD_LINE_BUFFER_SIZE];
uint32_t display_com_timeout = 0;
uint32_t welcome_screen_timeout = 0;
uint32_t calibration_done_screen_timeout = 0;

TelescopeMessageType TelescopeData;

float display_speed_ref = -2;
float display_speed_ref_tmp;
float display_speed_ref_increment = 1;

void DisplayHandler(void)
{
  DisplayStateMachineHandler();
  BrightnessHandler();
}

void DisplayTimeHandler(void)
{
  display_com_timeout++;
  welcome_screen_timeout++;
  calibration_done_screen_timeout++;
  
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

void DisplaySwitchDiagPage(DisplayDiagPageType NewPage)
{
  DisplayDiagPage = NewPage;
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
  if (DisplayState == DISPLAY_STATE_DIAGNOSTICS)
  {
    if (DisplayDiagPage == DISPLAY_DIAG_PAGE_SPEED)
      DisplaySwitchDiagPage(DISPLAY_DIAG_PAGE_POSITION);
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_MOTOR)
      DisplaySwitchDiagPage(DISPLAY_DIAG_PAGE_SPEED);
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_BATTERIES)
      DisplaySwitchDiagPage(DISPLAY_DIAG_PAGE_MOTOR);
  }
  else if (DisplayComTimeout != DISPLAY_COM_TIMEOUT && DisplayState == DISPLAY_STATE_SET_SPEED)
  {
    if (display_speed_ref_increment < 0.5)
      display_speed_ref_increment = 10 * display_speed_ref_increment;
  }
}

void DisplayRightButtonPressed(void)
{  
  if (DisplayState == DISPLAY_STATE_DIAGNOSTICS)
  {
    if (DisplayDiagPage == DISPLAY_DIAG_PAGE_POSITION)
      DisplaySwitchDiagPage(DISPLAY_DIAG_PAGE_SPEED);
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_SPEED)
      DisplaySwitchDiagPage(DISPLAY_DIAG_PAGE_MOTOR);
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_MOTOR)
      DisplaySwitchDiagPage(DISPLAY_DIAG_PAGE_BATTERIES);
  }
  else if (DisplayComTimeout != DISPLAY_COM_TIMEOUT && DisplayState == DISPLAY_STATE_SET_SPEED)
  {
    if (display_speed_ref_increment > 0.0005)
      display_speed_ref_increment = display_speed_ref_increment / 10;
  }
}

void DisplayUpButtonPressed(void)
{
  if (DisplayComTimeout == DISPLAY_COM_TIMEOUT || DisplayState != DISPLAY_STATE_SET_SPEED)
  {
    IncreaseBrightness();
  }
  else if (DisplayComTimeout != DISPLAY_COM_TIMEOUT && DisplayState == DISPLAY_STATE_SET_SPEED)
  {
    display_speed_ref_tmp += display_speed_ref_increment;
    
    if (display_speed_ref_tmp > 2)
      display_speed_ref_tmp = 2;
  }
}

void DisplayDownButtonPressed(void)
{
  if (DisplayComTimeout == DISPLAY_COM_TIMEOUT || DisplayState != DISPLAY_STATE_SET_SPEED)
  {
    DecreaseBrightness();
  }
  else if (DisplayComTimeout != DISPLAY_COM_TIMEOUT && DisplayState == DISPLAY_STATE_SET_SPEED)
  {
    display_speed_ref_tmp -= display_speed_ref_increment;
    
    if (display_speed_ref_tmp < 0)
      display_speed_ref_tmp = 0;
  }
}

void DisplayOkButtonPressed(void)
{
  if (DisplayComTimeout != DISPLAY_COM_TIMEOUT)
  {
    if (DisplayState == DISPLAY_STATE_DIAGNOSTICS)
    {
      display_speed_ref_tmp = TelescopeData.SpeedControllerData.controller_speed_ref;
      DisplaySwitchState(DISPLAY_STATE_SET_SPEED);
    }
    else if (DisplayState == DISPLAY_STATE_SET_SPEED)
    {
      display_speed_ref = display_speed_ref_tmp;
      DisplaySwitchState(DISPLAY_STATE_DIAGNOSTICS);
    }
    else if (DisplayState == DISPLAY_STATE_START_CALIBRATION)
    {
      display_speed_ref = -1;
    }
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
        stage_2_state = (int)(102 * TelescopeData.SpeedControllerData.actual_pos);
        
        if (stage_2_state > 100)
          stage_2_state = 100;
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
      calibration_done_screen_timeout = 0;
      DisplaySwitchState(DISPLAY_STATE_CALIBRATION_DONE);
    }
    display_speed_ref = -2;
    display_speed_ref_tmp = -2;
  }
  else if (DisplayState == DISPLAY_STATE_CALIBRATION_DONE)
  {
    LCD_LOCATE(2, 1);
    sprintf(LcdLineBuffer, "Calibration\n");
    LCD_printstring(LcdLineBuffer);
    
    LCD_LOCATE(3, 1);
    sprintf(LcdLineBuffer, "was successful!\n");
    LCD_printstring(LcdLineBuffer);  
    
    if (calibration_done_screen_timeout > CALIBRATION_DONE_TIME)
    {
      display_speed_ref = -2;
      display_speed_ref_tmp = -2;
      DisplaySwitchState(DISPLAY_STATE_DIAGNOSTICS);
    }    
  }
  else if (DisplayState == DISPLAY_STATE_DIAGNOSTICS)
  {    
    if (TelescopeData.SpeedControllerData.SpeedControllerState != SPEED_CONTROLLER_OK)
    {
      DisplaySwitchState(DISPLAY_STATE_START_CALIBRATION);
      return;
    }
    
    if (DisplayDiagPage == DISPLAY_DIAG_PAGE_POSITION)
    {
      LCD_LOCATE(1, 1);
      sprintf(LcdLineBuffer, "Position\n");
      LCD_printstring(LcdLineBuffer);
            
      LCD_LOCATE(2, 2);
      sprintf(LcdLineBuffer, "exp: %8.4f\n", TelescopeData.SpeedControllerData.expected_pos);
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(3, 2);
      sprintf(LcdLineBuffer, "act: %8.4f\n", TelescopeData.SpeedControllerData.actual_pos);
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(4, 2);
      sprintf(LcdLineBuffer, "err: %8.4f\n", TelescopeData.SpeedControllerData.position_error_whole_turn);
      LCD_printstring(LcdLineBuffer);
    }
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_SPEED)
    {
      LCD_LOCATE(1, 1);
      sprintf(LcdLineBuffer, "Speed\n");
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(2, 2);
      sprintf(LcdLineBuffer, "ref: %8.4f\n", TelescopeData.SpeedControllerData.controller_speed_ref);
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(3, 2);
      sprintf(LcdLineBuffer, "act: %8.4f\n", TelescopeData.SpeedControllerData.motor_speed_1sec_filtered);
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(4, 2);
      sprintf(LcdLineBuffer, "avg: %8.4f\n", TelescopeData.SpeedControllerData.motor_speed_whole_turn);
      LCD_printstring(LcdLineBuffer);
    }
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_MOTOR)
    {
      LCD_LOCATE(1, 1);
      sprintf(LcdLineBuffer, "Motor\n");
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(2, 2);
      sprintf(LcdLineBuffer, "enc:%8.4f\n", TelescopeData.SpeedControllerData.motor_speed_1sec);
      LCD_printstring(LcdLineBuffer);
            
      LCD_LOCATE(3, 2);
      sprintf(LcdLineBuffer, "pwm:%8d\n", (unsigned)TelescopeData.SpeedControllerData.motor_pwm);
      LCD_printstring(LcdLineBuffer);
    }
    else if (DisplayDiagPage == DISPLAY_DIAG_PAGE_BATTERIES)
    {
      LCD_LOCATE(1, 1);
      sprintf(LcdLineBuffer, "Batteries\n");
      LCD_printstring(LcdLineBuffer);
             
      float battery_voltage = GetFilteredBatteryVoltage();
      
      LCD_LOCATE(2, 2);
      sprintf(LcdLineBuffer, "rmt: %6.3fV\n", battery_voltage);
      LCD_printstring(LcdLineBuffer);
      
      LCD_LOCATE(3, 2);
      sprintf(LcdLineBuffer, "scp: %6.3fV\n", TelescopeData.battery_voltage);
      LCD_printstring(LcdLineBuffer);
    }
  }
  else if (DisplayState == DISPLAY_STATE_SET_SPEED)
  {
    if (TelescopeData.SpeedControllerData.SpeedControllerState != SPEED_CONTROLLER_OK)
    {
      DisplaySwitchState(DISPLAY_STATE_START_CALIBRATION);
      return;
    }
    
    LCD_LOCATE(1, 2);
    sprintf(LcdLineBuffer, "Speed settings\n");
    LCD_printstring(LcdLineBuffer);
    
    LCD_LOCATE(3, 4);
    sprintf(LcdLineBuffer, "%8.4f \n", display_speed_ref_tmp);
    LCD_printstring(LcdLineBuffer);
        
    LCD_LOCATE(4, 4);
    sprintf(LcdLineBuffer, "%8.4f\n", display_speed_ref_increment);
    
    for (int i = 0; i < strlen(LcdLineBuffer) - 1; i++)
    {
      if (LcdLineBuffer[i] == '1')
        LcdLineBuffer[i] = '^';
      else
        LcdLineBuffer[i] = ' ';
    }
    
    LCD_printstring(LcdLineBuffer);
  }
}

float DisplayGetSpeedRef(void)
{
  return display_speed_ref;
}
