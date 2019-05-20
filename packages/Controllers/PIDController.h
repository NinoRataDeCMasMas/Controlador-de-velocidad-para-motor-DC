/**
 * @file   PIDController.h
 * @date   Abr 22, 2019
 * @brief  Code for the PID controller by Arduino.
 */
#include "Controller.h"

class PIDController : public Controller {
  unsigned long lastTime;
  double lastErr, Kd, errSum, Ki; // PID params
  
public:
  PIDController(double* setpoint, double* input, double* output, double Kp, double Kd, double Ki);
  void compute(void);
};

PIDController::PIDController(double* setpoint, double* input, double* output, double Kp, double Kd, double Ki): Controller(setpoint, input, output, Kp)
{
  this->lastTime = 0;  
  this->Ki = Ki;
  this->Kd = Kd;
}

void PIDController::compute(void)
{
  // How much time has passed since the last compute
  unsigned long now = millis();
  double deltaTime  = (double)(now - lastTime);

  // We calculate the error variables
  double error = *setpoint - *input;
  double dErr  = (error - lastErr) / deltaTime;
  errSum += (error * deltaTime);
  
  *output = Kp * error + Ki * errSum + Kd * dErr;

  // We save the value of some variables for the next compute cycle
  lastErr  = error;
  lastTime = now;
}
/* EOF */
