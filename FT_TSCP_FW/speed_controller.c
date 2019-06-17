#include "speed_controller.h"

#include "bsp_pwm_timer.h"
#include "bsp_encoder.h"
#include "bsp_timing_delay.h"

SpeedControllerType SpeedController;

uint32_t speed_controller_1sec_cycle = 0;
uint32_t speed_controller_whole_turn_cycle = 0;

uint32_t speed_controller_1sec_max_cycle = 1000;

uint16_t last_encoder = 0;
uint16_t last_encoder_1sec = 0;
uint16_t last_encoder_whole_turn = 0;

uint32_t increment_integral = 0;
uint32_t increment_integral_linearized = 0;
uint32_t increment_integral_offset = 0;

uint32_t last_increment_integral_linearized_1sec = 0;

float expected_base_position = 0;
float expected_increment_ms = 0;
uint32_t expected_increment_cycle = 0;

float speed_error_integral = 0;
float position_error_correction = 1;

uint32_t calibration_cycle = 0;

float speed_correction_array[SPEED_CORRECTION_SIZE];
float speed_correction_array_filtered[SPEED_CORRECTION_SIZE];
float speed_correction_characteristic[SPEED_CORRECTION_SIZE];

void InitSpeedController(void)
{  
  SpeedController.SpeedControllerState = SPEED_CONTROLLER_NOT_INITED;
  
  SpeedController.motor_speed_1sec = 0;
  SpeedController.motor_speed_1sec_filtered = 0;
  SpeedController.motor_speed_whole_turn = 0;
  SpeedController.controller_speed_ref = 0;
  SpeedController.controller_speed_ref_with_offset = 0;
  SpeedController.expected_pos = 0;
  SpeedController.actual_pos = 0;
  SpeedController.position_error_whole_turn = 0;
  
  position_error_correction = 1;
  
  for (int i = 0; i < SPEED_CORRECTION_SIZE; i++)
  {
    speed_correction_characteristic[i] = (i + 1) * ((int)ENCODER_INCREMENT_PER_ROTATION / SPEED_CORRECTION_SIZE);
  }
}

SpeedControllerType GetSpeedController(void)
{  
  SpeedController.motor_pwm = GetPwm();  
  return SpeedController;
}

void SetSpeedRef(float speed_ref)
{
  if (speed_ref < 0)
  {
    if ((speed_ref == -1) && (SpeedController.SpeedControllerState == SPEED_CONTROLLER_NOT_INITED))
      SpeedController.SpeedControllerState = SPEED_CONTROLLER_INITED;
    return;
  }
  
  if (SpeedController.SpeedControllerState != SPEED_CONTROLLER_OK)
    return;
  
  float speed_ref_diff = speed_ref - SpeedController.controller_speed_ref;
  
  if (speed_ref < 0.0001)
  {
    SpeedController.motor_speed_1sec = 0;
    SpeedController.motor_speed_1sec_filtered = 0;
    SpeedController.motor_speed_whole_turn = 0;
    SpeedController.controller_speed_ref = 0;
    SpeedController.controller_speed_ref_with_offset = 0;
    SpeedController.expected_pos = 0;
    SpeedController.actual_pos = 0;
    SpeedController.position_error_whole_turn = 0;
    
    speed_controller_whole_turn_cycle = 0;
    last_encoder_whole_turn = GetEncoderValue();
    
    speed_error_integral = 0;    
    position_error_correction = 1;
    
    increment_integral_offset = increment_integral_linearized;
    
    expected_base_position = 0;
    expected_increment_ms = 0;
    expected_increment_cycle = 0;
    
    SetPwm(0);
    return;
  }
  
  if (speed_ref_diff == 0)
    return;
  
  SpeedController.controller_speed_ref_with_offset += speed_ref_diff;
  SpeedController.controller_speed_ref +=speed_ref_diff;
  
  expected_base_position = SpeedController.expected_pos;
  expected_increment_cycle = 0;
  expected_increment_ms = (double)speed_ref / MILLISECS_PER_MIN;
}

float GetSpeedRef(void)
{
  return SpeedController.controller_speed_ref;
}

void SpeedControllerHandler(void)
{    
  uint16_t actual_encoder = GetEncoderValue();
  
  uint16_t difference = actual_encoder - last_encoder;
  increment_integral += difference;
  
  increment_integral_linearized = GetLinearizedIncrement(increment_integral);
  
  SpeedController.actual_pos = (float)(increment_integral_linearized - increment_integral_offset) / (float)ENCODER_INCREMENT_PER_ROTATION;
  last_encoder = actual_encoder;

  SpeedController.expected_pos = expected_base_position + expected_increment_cycle * expected_increment_ms;
  
  if (speed_controller_1sec_cycle == speed_controller_1sec_max_cycle - 1)
  {   
    uint16_t difference_1sec = 0;
    
    if (SpeedController.SpeedControllerState != SPEED_CONTROLLER_OK)
      difference_1sec = actual_encoder - last_encoder_1sec;
    else
      difference_1sec = increment_integral_linearized - last_increment_integral_linearized_1sec;
    
    SpeedController.motor_speed_1sec = (float)difference_1sec / (float)speed_controller_1sec_cycle;
    SpeedController.motor_speed_1sec = (MILLISECS_PER_MIN / (float)ENCODER_INCREMENT_PER_ROTATION) * SpeedController.motor_speed_1sec;
    
    SpeedController.motor_speed_1sec_filtered = (1 - SPEED_1S_FILTER_WEIGHT) * SpeedController.motor_speed_1sec_filtered;
    SpeedController.motor_speed_1sec_filtered += SPEED_1S_FILTER_WEIGHT * SpeedController.motor_speed_1sec;
    
    last_encoder_1sec = actual_encoder;
    last_increment_integral_linearized_1sec = increment_integral_linearized;
    speed_controller_1sec_cycle = 0;
    
    if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_OK)
    {    
      if (SpeedController.actual_pos > CORRECTION_START_POS)
      {
        position_error_correction = SpeedController.position_error_whole_turn;
            
        if (position_error_correction < 0)
          position_error_correction = -position_error_correction;
      }
      
      float k_p = CONTROLLER_KP * (position_error_correction + CORRECTION_MIN);
      float k_i = CONTROLLER_KI * (position_error_correction + CORRECTION_MIN);
      
      float actual_position_error = SpeedController.expected_pos - SpeedController.actual_pos;
      
      float speed_error = SpeedController.controller_speed_ref_with_offset - SpeedController.motor_speed_1sec;
      speed_error += POS_ERROR_KP * (position_error_correction + CORRECTION_MIN) * actual_position_error;
      
      speed_error_integral += k_i * speed_error;
      
      if (speed_error_integral < 0)
        speed_error_integral = 0;
      
      float new_pwm = k_p * speed_error + speed_error_integral;    
      SetPwm((int32_t)new_pwm);
    }
  }
    
  uint16_t difference_whole_turn = actual_encoder - last_encoder_whole_turn;
  
  if (difference_whole_turn >= ENCODER_INCREMENT_PER_ROTATION)
  {
    SpeedController.motor_speed_whole_turn = (float)difference_whole_turn / (float)speed_controller_whole_turn_cycle;
    SpeedController.motor_speed_whole_turn = (MILLISECS_PER_MIN / (float)ENCODER_INCREMENT_PER_ROTATION) * SpeedController.motor_speed_whole_turn;
    
    if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_OK)
    {
      SpeedController.position_error_whole_turn = SpeedController.expected_pos - SpeedController.actual_pos;
      SpeedController.controller_speed_ref_with_offset = SpeedController.controller_speed_ref + SpeedController.position_error_whole_turn / SPEED_REF_COR_DIV;
    }

    last_encoder_whole_turn = actual_encoder;
    speed_controller_whole_turn_cycle = 0;
  }
  
  if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_INITED)
  {
    calibration_cycle++;
    
    if (calibration_cycle > INIT_WAIT_TIME)
    {
      calibration_cycle = 0;
      SpeedController.SpeedControllerState = SPEED_CONTROLLER_CALIBRATION_SEARCH_PWM;
    }
  }
  else if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_CALIBRATION_SEARCH_PWM)
  {    
    if (SpeedController.motor_speed_1sec < CALIBRATION_MIN_SPEED)
    {
      calibration_cycle++;
      SetPwm(calibration_cycle / 5);
    }
    
    if (SpeedController.actual_pos > PWM_SEARCH_ROTATION)
    {
      SpeedController.SpeedControllerState = SPEED_CONTROLLER_CALIBRATION_START;
    }
  }
  else if(SpeedController.SpeedControllerState == SPEED_CONTROLLER_CALIBRATION_START)
  {
    if (increment_integral >= ENCODER_INCREMENT_PER_ROTATION)
    {
      increment_integral = 0;
      SpeedController.SpeedControllerState = SPEED_CONTROLLER_CALIBRATION_IN_PROGRESS;
    }
  }
  else if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_CALIBRATION_IN_PROGRESS)
  {
    if (increment_integral < ENCODER_INCREMENT_PER_ROTATION)
    {
      int index = increment_integral / ((int)ENCODER_INCREMENT_PER_ROTATION / SPEED_CORRECTION_SIZE);
      speed_correction_array[index] = SpeedController.motor_speed_1sec;
    }
    else
    {
      SpeedController.SpeedControllerState = SPEED_CONTROLLER_CALIBRATION_FINISHED;
    }
  }
  
  speed_controller_1sec_cycle++;
  speed_controller_whole_turn_cycle++;
  
  if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_OK)
  {
    expected_increment_cycle++;
  }
}

void SpeedControllerLoop(void)
{
  if (SpeedController.SpeedControllerState == SPEED_CONTROLLER_CALIBRATION_FINISHED)
  {
    for (int i = 0; i < SPEED_CORRECTION_SIZE; i++)
    {
      float filtered_value = 0;
      
      for (int j = 0; j < SPEED_CORRECTION_FILTER_SIZE; j++)
      {
        int index = i + j - (SPEED_CORRECTION_FILTER_SIZE / 2);
        
        if (index < 0)
          index += SPEED_CORRECTION_SIZE;
        else if (index >= SPEED_CORRECTION_SIZE)
          index -= SPEED_CORRECTION_SIZE;
        
        filtered_value += speed_correction_array[index];
      } 
      
      speed_correction_array_filtered[i] = filtered_value / SPEED_CORRECTION_FILTER_SIZE;
      speed_correction_array_filtered[i] = 1 / speed_correction_array_filtered[i];
    }
        
    for (int i = 0; i < SPEED_CORRECTION_SIZE; i++)
    {
      float value = 0;
      
      for (int j = 0; j <= i; j++)
      {
        value += speed_correction_array_filtered[j];
      }
      
      speed_correction_characteristic[i] = value;
    }
    
    for (int i = 0; i < SPEED_CORRECTION_SIZE; i++)
    {
      speed_correction_characteristic[i] = speed_correction_characteristic[i] / speed_correction_characteristic[SPEED_CORRECTION_SIZE - 1];
      speed_correction_characteristic[i] = ENCODER_INCREMENT_PER_ROTATION * speed_correction_characteristic[i];
    }
    
    SetPwm(0);    
    Delay(2000);
    
    increment_integral_offset = increment_integral_linearized;
    
    SpeedController.motor_speed_1sec_filtered = 0;
    SpeedController.motor_speed_whole_turn = 0;
    SpeedController.SpeedControllerState = SPEED_CONTROLLER_OK;
  }
}

uint32_t GetLinearizedIncrement(uint32_t inc_integral)
{
  uint32_t increment_integral_base = (uint32_t)ENCODER_INCREMENT_PER_ROTATION * (inc_integral / ((uint32_t)ENCODER_INCREMENT_PER_ROTATION));
  uint32_t increment_in_rotation = inc_integral % ((uint32_t)ENCODER_INCREMENT_PER_ROTATION);
  
  uint32_t characteristic_quantation = (uint32_t)ENCODER_INCREMENT_PER_ROTATION / SPEED_CORRECTION_SIZE;
	
  int index = increment_in_rotation / characteristic_quantation;
  
  float left_value = 0;
  float right_value = speed_correction_characteristic[index];
  
  if (index > 0)
    left_value = speed_correction_characteristic[index - 1];
  
  float right_weight = increment_in_rotation % characteristic_quantation;
  float left_weight = characteristic_quantation - right_weight;
  
  uint32_t linear_increment_in_rotation = (uint32_t)((left_weight * left_value + right_weight * right_value) / characteristic_quantation);
  
  return increment_integral_base + (uint32_t)linear_increment_in_rotation;
}
