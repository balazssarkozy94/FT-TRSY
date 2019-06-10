#include "speed_controller.h"

#include "bsp_pwm_timer.h"
#include "bsp_encoder.h"

SpeedControllerType SpeedController;

uint32_t speed_controller_1sec_cycle = 0;
uint32_t speed_controller_whole_turn_cycle = 0;

uint32_t speed_controller_1sec_max_cycle = 1000;

uint16_t last_encoder = 0;
uint16_t last_encoder_1sec = 0;
uint16_t last_encoder_whole_turn = 0;

uint32_t increment_integral = 0;

float expected_base_position = 0;
float expected_increment_ms = 0;
uint32_t expected_increment_cycle = 0;

float speed_error_integral = 0;

void InitSpeedController(void)
{
  SpeedController.motor_speed_1sec = 0;
  SpeedController.motor_speed_whole_turn = 0;
  SpeedController.controller_speed_ref = 0;
  SpeedController.controller_speed_ref_with_offset = 0;
  SpeedController.expected_pos = 0;
  SpeedController.actual_pos = 0;
  SpeedController.position_error_whole_turn = 1;
  
  SetSpeedRef(0.7);
}

SpeedControllerType GetSpeedController(void)
{  
  SpeedController.motor_pwm = GetPwm();
  
  return SpeedController;
}

void SetSpeedRef(float speed_ref)
{
  float speed_ref_diff = speed_ref - SpeedController.controller_speed_ref;
  
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
  SpeedController.actual_pos = (float)increment_integral / (float)ENCODER_INCREMENT_PER_ROTATION;
  last_encoder = actual_encoder;

  SpeedController.expected_pos = expected_base_position + expected_increment_cycle * expected_increment_ms;
  
  if (speed_controller_1sec_cycle == speed_controller_1sec_max_cycle - 1)
  {   
    uint16_t difference_1sec = actual_encoder - last_encoder_1sec;
    
    SpeedController.motor_speed_1sec = (float)difference_1sec / (float)speed_controller_1sec_cycle;
    SpeedController.motor_speed_1sec = (MILLISECS_PER_MIN / ENCODER_INCREMENT_PER_ROTATION) * SpeedController.motor_speed_1sec;
    
    last_encoder_1sec = actual_encoder;
    speed_controller_1sec_cycle = 0;
    
    float correction = SpeedController.position_error_whole_turn;
        
    if (correction < 0)
      correction = - correction;
    
    float k_p = 150 * (correction + 0.1);
    float k_i = 1000 * (correction + 0.2);
    
    float actual_position_error = SpeedController.expected_pos - SpeedController.actual_pos;
    
    float speed_error = SpeedController.controller_speed_ref_with_offset - SpeedController.motor_speed_1sec;
    speed_error += 0.5 * (correction + 0.1) * actual_position_error;
    
    speed_error_integral += k_i * speed_error;
    
    float new_pwm = k_p * speed_error + speed_error_integral;
    SetPwm((int32_t)new_pwm);
  }
    
  uint16_t difference_whole_turn = actual_encoder - last_encoder_whole_turn;
  
  if (difference_whole_turn >= ENCODER_INCREMENT_PER_ROTATION)
  {
    SpeedController.motor_speed_whole_turn = (float)difference_whole_turn / (float)speed_controller_whole_turn_cycle;
    SpeedController.motor_speed_whole_turn = (MILLISECS_PER_MIN / ENCODER_INCREMENT_PER_ROTATION) * SpeedController.motor_speed_whole_turn;
    
    SpeedController.position_error_whole_turn = SpeedController.expected_pos - SpeedController.actual_pos;
    SpeedController.controller_speed_ref_with_offset = SpeedController.controller_speed_ref + SpeedController.position_error_whole_turn / 5;
    
    last_encoder_whole_turn = actual_encoder;
    speed_controller_whole_turn_cycle = 0;
  }
  
  speed_controller_1sec_cycle++;
  speed_controller_whole_turn_cycle++;
  expected_increment_cycle++;
}