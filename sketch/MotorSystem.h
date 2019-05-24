/**
 * @file   MotorSystem.h
 * @date   May 21,  2019
 * @brief  Code to control the motor system.    
 */
#include <Printer.h>
#include <Encoder.h>
#include <Controller.h>
#include <PIController.h>
#include <PDController.h>
#include <PIDController.h>
#include <LiquidCrystal_I2C.h>
#include <ComplementaryFilter.h>
/**
 * @brief MotorSystem class
 */
class MotorSystem
{
  const int offset = 50;          // offset to use potentiometer as on/off switch
  byte motorPin, potPin;          // pines for potentiomerer and motor control
  byte encoderPin1, encoderPin2;  // pines to read response by encoder 
  double setpoint, input, output; // variables for controller
  
  Encoder* encoder;               // instance of encoder class; read by interruption pin
  LiquidCrystal_I2C* lcd;         // instance of lcd class: show input and output variables
  ComplementaryFilter* filter;    // instance of filter class: filtering potentiometer signal
  Controller* controller;         // instance of controller class: PID controller implementation
  
  void debugger() const;          // method to observe the inner system variables
  void plot() const;              // method to plot setpoint vs output  
  void runWithFeedback();         // control system with feedback
  void runWithoutFeedback();      // control system without feedback  
  
public:
  MotorSystem(byte motorPin, byte potPin, byte encoderPin1, byte encoderPin2); // ctor1
	MotorSystem(byte motorPin, byte potPin, byte encoderPin1, byte encoderPin2, Controller* control); // ctor2
  
	byte interruptPin() const;      // provides the encoder pin to configure arduino's interruption
	void generateCuerve() const;    // routine to generate a system's curve to analysis
	void begin();                   // provides the encoder method to use in arduino's isr
	void run();                     // run the control system
};
/**
 * @brief Parametrized ctor.
 * 
 * @param motorPin:    pin of motor 
 * @param potPin:      pin of potentiometer
 * @param encoderPin1: pin of first encoder/ interruption pin
 * @param encoderPin2: pin of second encoder
 */
MotorSystem::MotorSystem(byte motorPin, byte potPin, byte encoderPin1, byte encoderPin2):
motorPin(motorPin), potPin(potPin), encoderPin1(encoderPin1), encoderPin2(encoderPin2)
{
  setpoint   = input = output = 0.0; // init the controller variables
  controller = NULL; // the controller is not required in this funcionality
   
  // init the system components  
  encoder    = new Encoder(encoderPin1, encoderPin2);
  lcd        = new LiquidCrystal_I2C(0x27, 16, 2);
  filter     = new ComplementaryFilter(0.75);
  
  // configure some system components
  lcd->begin();           // enable lcd screen
  pinMode(potPin, INPUT); // enable potentiometer pin
}
/**
 * @brief Parametrized ctor.
 * 
 * @param motorPin:    pin of motor 
 * @param potPin:      pin of potentiometer
 * @param encoderPin1: pin of first encoder/ interruption pin
 * @param encoderPin2: pin of second encoder
 * @param control:     Controller for feedback
 */
MotorSystem::MotorSystem(byte motorPin, byte potPin, byte encoderPin1, byte encoderPin2, Controller* control):
motorPin(motorPin), potPin(potPin), encoderPin1(encoderPin1), encoderPin2(encoderPin2)
{
	setpoint   = input = output = 0.0; // init the controller variables
  controller = control; // the controller is required for feedback
   
  // init the system components  
	encoder    = new Encoder(encoderPin1, encoderPin2);
	lcd        = new LiquidCrystal_I2C(0x27, 16, 2);
  filter     = new ComplementaryFilter(0.75);    
	
	// configure some system components
  lcd->begin();           // enable lcd screen
  pinMode(potPin, INPUT); // enable potentiometer pin
  controller->attach(&setpoint, &input, &output); // nessesary params for Controller class
}
/**
 * @brief method to observe the inner system variables
 */
void MotorSystem::debugger() const
{
  Serial << setpoint << ", " << input << ", " << setpoint - input << ", " << output << endl; 
}
/**
 * @brief method to plot setpoint vs output
 */
void MotorSystem::plot() const
{
  Serial << setpoint << "," << output << "," << input << endl;
}
/**
 * @brief provides the encoder method to use in arduino's isr
 */
void MotorSystem::begin( void )
{
	encoder->read();
}
/**
 * @brief  provides the encoder pin to configure arduino's interruption
 * @return encoderPin1: attached pin to encoder
 */
byte MotorSystem::interruptPin() const
{
	return encoderPin1;  
}
/**
 * @brief control system with feedback
 */
void MotorSystem::runWithFeedback()
{
  // we read revolution value from potentiometer. the value contains
  // an offset that serve as on/off switch.
  auto pot = map(analogRead(potPin), 0, 1023, 0, 587 + offset); // 0 to 587 is the linear region of motor  system 
                                                                // revolutions, measured in pulses per delta-time
  if(pot < offset)
  {
    // A "pot" value less than offset indicates system in off mode
    analogWrite(motorPin, 0);
    // show data in lcd screen
    lcd->setCursor(0, 0); lcd->print("*  controlador *");
    lcd->setCursor(0, 1); lcd->print("*  de motor DC *");         
  }
  else
  {
    // A "pot" value greater than offset indicates system in on mode.
    setpoint = filter->compute(pot - (offset + .0));  // filtering pot value without offset and set this
                                                      // value as stepoint 
    input    = encoder->pulseXsec();                  // input works as feedback loop
    controller->compute();                            // compute e(z) = setpoint - input -> k*e(z) = output
    output   = 0.131959*output + 22.318997;           // convert pulses per-delta to pwm  
    analogWrite(motorPin, output);                    // inject the response into the system
    
    output = 7.51300*output - 164.67621;              // convert pwm to pulses per-delta 
    plot();                                           // observe the inner system variables
    
    // show data in lcd screen
    lcd->setCursor(0, 0); lcd->print("setpoint: " + (String)setpoint + "  ");  
    lcd->setCursor(0, 1); lcd->print("output:   " + (String)output   + "  ");
  }  
}
/**
 * @brief control system without feedback
 */
void MotorSystem::runWithoutFeedback()
{
  // we read pwm value from potentiometer. the value contains
  // an offset that serve as on/off switch.
  auto pot = map(analogRead(potPin), 0, 1023, 0, 255 + offset); // 0 to 255 is the pwm range

  if(pot < offset)
  {
    // A "pot" value less than offset indicates system in off mode
    analogWrite(motorPin, 0);
    // show data in lcd screen
    lcd->setCursor(0, 0); lcd->print("#  controlador #");
    lcd->setCursor(0, 1); lcd->print("#  de motor DC #");             
  }
  else
  {
    // A "pot" value greater than offset indicates system in on mode.
    // filtering pot value without offset    
    auto pwm = filter->compute(pot - (offset + .0));  
    analogWrite(motorPin, pwm);
    // show data in lcd screen
    lcd->setCursor(0, 0); lcd->print("setpoint: " + (String)pwm                  + "    ");  
    lcd->setCursor(0, 1); lcd->print("output:   " + (String)encoder->pulseXsec() + "    ");    
  }  
} 
/**  
 *   @breaf run the control system
 */
void MotorSystem::run( void )
{
  if(controller == NULL) runWithoutFeedback(); // if the object are built with ctor1
  else runWithFeedback();                      // if the object are built with ctor2
}
/**
 * @brief routine to generate a system's curve to analysis
 */
void MotorSystem::generateCuerve( void ) const
{
	auto range = 255;
  // read the response of the system to the imposed range
	for(int pwm = 0; pwm <= range; ++pwm)
	{
		analogWrite(motorPin, pwm);
		Serial << pwm << "," << encoder->pulseXsec() << endl;  
		delay(50);
	}

	delay(50);
	analogWrite(motorPin, 0); // turn off system
	while(true);              // stop measure
}
/* EOF */
