#include "speed_controller.h"

#include "bsp_pwm_timer.h"
#include "bsp_encoder.h"

SpeedControllerType SpeedController;

uint32_t speed_controller_5sec_cycle = 0;
uint32_t speed_controller_whole_turn_cycle = 0;

uint32_t speed_controller_5sec_max_cycle = 5000;

uint16_t last_encoder = 0;
uint16_t last_encoder_5sec = 0;
uint16_t last_encoder_whole_turn = 0;

uint32_t increment_integral = 0;

double expected_position = 0;
double expected_increment_ms = 0;

float speed_error_integral = 0;

void InitSpeedController(void)
{
  SpeedController.motor_speed_5sec = 0;
  SpeedController.motor_speed_whole_turn = 0;
  SpeedController.controller_speed_ref = 0;
  SpeedController.controller_speed_ref_with_offset = 0;
  SpeedController.expected_pos = 0;
  SpeedController.actual_pos = 0;
  SpeedController.position_error_whole_turn = 1;
  
  SetSpeedRef(0.5);
}

SpeedControllerType GetSpeedController(void)
{  
  SpeedController.motor_pwm = GetPwm();
  
  return SpeedController;
}

void SetSpeedRef(float speed_ref)
{
  SpeedController.controller_speed_ref_with_offset += (speed_ref - SpeedController.controller_speed_ref);
  SpeedController.controller_speed_ref = speed_ref;
  
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
  
  expected_position += expected_increment_ms;
  SpeedController.expected_pos = (float)expected_position;
  
  if (speed_controller_5sec_cycle == speed_controller_5sec_max_cycle - 1)
  {   
    uint16_t difference_5sec = actual_encoder - last_encoder_5sec;
    
    SpeedController.motor_speed_5sec = (float)difference_5sec / (float)speed_controller_5sec_cycle;
    SpeedController.motor_speed_5sec = (MILLISECS_PER_MIN / ENCODER_INCREMENT_PER_ROTATION) * SpeedController.motor_speed_5sec;
    
    last_encoder_5sec = actual_encoder;
    speed_controller_5sec_cycle = 0;
    
    float speed_error = SpeedController.controller_speed_ref_with_offset - SpeedController.motor_speed_5sec;
    
    float correction = SpeedController.position_error_whole_turn;
    
    if (correction < 0)
      correction = - correction;
    
    correction += 0.2;
    
    float k_p = 250 * correction;
    float k_i = 5000 * correction;
    
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
    SpeedController.controller_speed_ref_with_offset = SpeedController.controller_speed_ref + SpeedController.position_error_whole_turn / 10;
    
    last_encoder_whole_turn = actual_encoder;
    speed_controller_whole_turn_cycle = 0;
  }
  
  speed_controller_5sec_cycle++;
  speed_controller_whole_turn_cycle++;
}