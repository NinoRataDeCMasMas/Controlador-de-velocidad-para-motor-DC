/**
 * @file   PDController.h
 * @date   Abr 22, 2019
 * @brief  Code for the PD controller by Arduino.
 */
#include "Controller.h"

class PDController : public Controller {
  unsigned long lastTime;
  double lastErr, Kd; // PID params
  
public:
  PDController(double* setpoint, double* input, double* output, double Kp, double Kd);
  void compute(void);  
};

PDController::PDController(double* setpoint, double* input, double* output, double Kp, double Kd): Controller(setpoint, input, output, Kp)
{
  this->lastTime = 0;
  this->Kd = Kd;  
}

void PDController::compute(void)
{
  // How much time has passed since the last compute
  unsigned long now = millis();
  double deltaTime  = (double)(now - lastTime);

  // We calculate the error variables
  double error = *setpoint - *input;
  double dErr  = (error - lastErr) / deltaTime;

  *output = Kp * error + Kd * dErr;

  // We save the value of some variables for the next compute cycle
  lastErr  = error;
  lastTime = now;
}
/* EOF */
