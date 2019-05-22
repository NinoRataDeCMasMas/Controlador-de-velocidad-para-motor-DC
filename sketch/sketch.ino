/**
 * @file   sketch.ino
 * @date   May 21,  2019
 * @brief  Code to control the motor system.    
 */
#include "MotorSystem.h"

MotorSystem* motorSystem;
/**
 * @brief  setup function.    
 */
void setup( void )
{
  const long baudage = 115200;
  
  Serial.begin(baudage);
  motorSystem = new MotorSystem(9, A1, 3, 4, new Controller(3));

  auto isr = []( void ) -> void { motorSystem->begin(); };
  attachInterrupt(digitalPinToInterrupt(motorSystem->interruptPin()), isr, CHANGE);
}
/**
 * @brief  loop function.    
 */
void loop( void ) { motorSystem->run(); }
