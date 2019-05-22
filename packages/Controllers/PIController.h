/**
 * @file   PIController.h
 * @date   Abr 22, 2019
 * @brief  Code for the PI controller by Arduino.
 */
#include "Controller.h"

class PIController : public Controller {
  unsigned long lastTime;
  double errSum, Ki; // PID params
  
public:
  PIController(double* setpoint, double* input, double* output, double Kp, double Ki);
  PIController(double Kp, double Ki);
  void compute(void); 
};

PIController::PIController(double* setpoint, double* input, double* output, double Kp, double Ki): Controller(setpoint, input, output, Kp)
{
  this->lastTime = 0;  
  this->Ki = Ki;  
}

PIController::PIController(double Kp, double Ki): Controller(Kp)
{
  this->lastTime = 0;  
  this->Ki = Ki;  
}

void PIController::compute(void)
{
  // How much time has passed since the last compute
  unsigned long now = millis();
  double deltaTime  = (double)(now - lastTime);

  // We calculate the error variables
  double error = *setpoint - *input;
  errSum += (error * deltaTime);

  *output = Kp * error + Ki * errSum;

  // We save the value of some variables for the next compute cycle
  lastTime = now;
}
/* EOF */
