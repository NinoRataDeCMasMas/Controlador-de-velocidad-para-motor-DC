/**
 * @file   Controller.h
 * @date   Abr 22, 2019
 * @brief  Code for the P controller by Arduino.
 */
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#include <Arduino.h>

class Controller {

protected:
  // System params
  double *setpoint, *input, *output;
  double Kp; // PID params
  
public:
  Controller(double* setpoint, double* input, double* output, double Kp);
  virtual void compute(void);
};

Controller::Controller(double* setpoint, double* input, double* output, double Kp)
{
  this->input    = input;
  this->setpoint = setpoint;
  this->output   = output;
  this->Kp = Kp;        
}

void Controller::compute(void)
{
  double error = *setpoint - *input;
  *output = Kp * error;
}

#endif /* _CONTROLLER_H_ */
/* EOF */
