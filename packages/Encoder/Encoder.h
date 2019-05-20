#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

class Encoder
{
  byte pinA, pinB;       // Encoder Pin A pin 2 and pin 3 are inturrpt pins; Encoder Pin B
  volatile long counts;  // counts the encoder counts. The encoder has ~233counts/rev

public:
  Encoder(int pinA, int pinB);
  long pulseXsec( void );
  void read( void );  
};
/**
 * @breaf Parametrized ctor.
 */
Encoder::Encoder(int pinA, int pinB):pinA(pinA), pinB(pinB)
{
  counts = 0;
  
  pinMode(pinA, INPUT);    // initialize Encoder Pins
  pinMode(pinB, INPUT);  
  digitalWrite(pinA, LOW); // initialize Pin States
  digitalWrite(pinB, LOW);
}
/**
 * @breaf This function is triggered by the encoder CHANGE,
 *        and increments the encoder counter. you may need
 *        to redefine positive and negative directions.
 */
void Encoder::read( void )
{ 
  bool isActivated = digitalRead(pinB) == digitalRead(pinA);
  counts = isActivated ? counts - 1 : counts + 1;
}
/**
 * @breaf  Read the pulses counted per 10 milliseconds
 * @return Lecture of pulses x 10 millis
 */
long Encoder::pulseXsec( void )
{
  auto previous = (counts > 0 ? counts : -counts) + .0;
  delay(100);
  auto current  = (counts > 0 ? counts : -counts) + .0;
  return current - previous;  
}
/* EOF */
