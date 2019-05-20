/**
 * @file   ComplementaryFilter.h
 * @date   Abr 22, 2019
 * @brief  Code for the complementary filter by Arduino.
 * @url    https://dsp.stackexchange.com/questions/25220/what-is-the-definition-of-a-complementary-filter
 */
class ComplementaryFilter {
  // The values of a and b would be respect this condition:
  // a + b = 1
  double a, b;
  double last;

public:
  ComplementaryFilter( double a );
  void set( double a );
  double compute( double curr );
};

ComplementaryFilter::ComplementaryFilter( double a )
{
  this->a = a;
  this->b = 1 - a;
  this->last  = 0; 
}

void ComplementaryFilter::set( double a )
{
  this->a = a;
  this->b = 1 - a;
  this->last  = 0; 
}

double ComplementaryFilter::compute( double curr )
{
  last = a*last + b*curr; // Complementary filter equation
  return last; // The  value of last is saved inside class 
}
/* EOF */


