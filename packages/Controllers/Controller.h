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
  Controller(double Kp);

  void attach(double* setpoint, double* input, double* output);
  virtual void compute(void);
};

Controller::Controller(double* setpoint, double* input, double* output, double Kp)
{
  this->input    = input;
  this->setpoint = setpoint;
  this->output   = output;
  this->Kp = Kp;        
}

Controller::Controller(double Kp)
{
  this->Kp = Kp;        
}

void Controller::attach(double* setpoint, double* input, double* output)
{
  this->input    = input;
  this->setpoint = setpoint;
  this->output   = output;
}

void Controller::compute(void)
{
  double error = *setpoint - *input;
  *output = Kp * error;
}

#endif /* _CONTROLLER_H_ */
/* EOF */
