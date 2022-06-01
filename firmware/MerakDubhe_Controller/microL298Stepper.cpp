/*
 * Transformed for L298
 * By Forrest Lee Erickson
 * Date: 20220601
 * 
 * Found at: https://forum.arduino.cc/t/h-bridge-microstepping-and-only-two-pwms/461671
  
  microStepper4.cpp - Library for stepper motor driving
  Created by San Zamoyski, 30.05.2017 

	based on

  Created by Clayton P. Webster, February 20th, 2010
  Released into the public domain
*/

#include "Arduino.h"
#include "microL298Stepper.h"


							//non-pwm	pwm		non-pwm		pwm
microStepper8::microStepper8(int coil1a, int coil1b, int coil2a, int coil2b)
{
  pinMode(coil1a, OUTPUT);
  pinMode(coil1b, OUTPUT);
  pinMode(coil2a, OUTPUT);
  pinMode(coil2b, OUTPUT);
  _coil1a = coil1a;
  _coil1b = coil1b;
  _coil2a = coil2a;
  _coil2b = coil2b;
  _counter = 1;

  /*_steps = {
    {1,   0,   0,   0},
    {1,   0,   1,  75},
    {1,   0,   1,   0},
    {1,  75,   1,   0},
    {0,   0,   1,   0},
    {0, 180,   1,   0},
    {0, 255,   1,   0},
    {0, 255,   1,  75},
    {0, 255,   0,   0},
    {0, 255,   0, 180},
    {0, 255,   0, 255},
    {0, 180,   0, 255},
    {0,   0,   0, 255},
    {1,  75,   0, 255},
    {1,   0,   0, 255},
    {1,   0,   0, 180}
  };*/

  
}


void microStepper8::disable()
{
  digitalWrite(_coil1a, LOW);
  digitalWrite(_coil1b, LOW);
  digitalWrite(_coil2a, LOW);
  digitalWrite(_coil2b, LOW);
}

void microStepper8::takestep(boolean invert)
{
    if (invert == false) { // CW
      _counter = _counter + 1;
      
      // roll over counter if they go too high
      if (_counter >= microsteps) {
        _counter = 0;
      }
    }
    else { // CCW
      _counter = _counter - 1;
      
      // reset counter if they go too low
      if (_counter < 0) {
        _counter = microsteps - 1;
      }
    }
    
	/*Serial.print(_counter);
	Serial.print(" => ");
	Serial.print(_steps[_counter][0]);
    Serial.print(" ");
    Serial.print(_steps[_counter][1]);
    Serial.print(" ");
    Serial.print(_steps[_counter][2]);
    Serial.print(" ");
    Serial.println(_steps[_counter][3]);*/

	// use analogWrite to create pwm for each MOSFET
    digitalWrite(_coil1a, _steps[_counter][0]);
    analogWrite(_coil1b, _steps[_counter][1]);
    digitalWrite(_coil2a, _steps[_counter][2]);
    analogWrite(_coil2b, _steps[_counter][3]);
}
