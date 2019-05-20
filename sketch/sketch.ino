#include "MotorSystem.h"

const long baudage = 115200;

MotorSystem* motorSystem;

void setup( void )
{
  Serial.begin(baudage);
  motorSystem = new MotorSystem(9, A1, 3, 4);

  auto isr = []( void ) -> void { motorSystem->begin(); };
  attachInterrupt(digitalPinToInterrupt(motorSystem->interruptPin()), isr, CHANGE);
}

void loop( void )
{
  motorSystem->run();
  //motorSystem->generateCuerve();
}
