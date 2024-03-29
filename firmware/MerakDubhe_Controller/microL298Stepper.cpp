/*
   Transformed for L298
   By Forrest Lee Erickson
   Date: 20220601

   Found at: https://forum.arduino.cc/t/h-bridge-microstepping-and-only-two-pwms/461671
  microStepper4.cpp - Library for stepper motor driving
  Created by San Zamoyski, 30.05.2017
	based on
  Created by Clayton P. Webster, February 20th, 2010
  Released into the public domain
*/

#include "Arduino.h"
#include "microL298Stepper.h"

//Pin pattern:  non-pwm non-pwm pwm   non-pwm   non-pwm  pwm
microL298Stepper::microL298Stepper(int coil1a, int coil1b, int pwm1, int coil2a, int coil2b, int pwm2)
{
  digitalWrite(coil1a, LOW);
  digitalWrite(coil1b, LOW);
  digitalWrite(coil2a, LOW);
  digitalWrite(coil2b, LOW);
  pinMode(coil1a, OUTPUT);
  pinMode(coil1b, OUTPUT);
  pinMode(coil2a, OUTPUT);
  pinMode(coil2b, OUTPUT);
  _coil1a = coil1a;
  _coil1b = coil1b;
  _coil2a = coil2a;
  _coil2b = coil2b;
  _pwm1 = pwm1;
  _pwm2 = pwm2;
  _counter = 0;
}//Class decleration

long microL298Stepper::counter(){
//int microL298Stepper::counter() {
  return (_counter);
}// end counter

//int * microL298Stepper::steps1024(int count){
byte * microL298Stepper::steps1024(int count) {
  return (_steps1024(count));
}// end counter


/*Set coils on and full PWM to act as electronic break to hold against rotation*/
void microL298Stepper::hold()
{
  digitalWrite(_coil1a, _steps[_counter][0]);
  digitalWrite(_coil1b, _steps[_counter][1]);
  analogWrite(_pwm1, _steps[_counter][2]);
  digitalWrite(_coil2a, _steps[_counter][3]);
  digitalWrite(_coil2b, _steps[_counter][4]);
  analogWrite(_pwm2, _steps[_counter][5]);
}//end hold()


/*Set coils off and PWM off to save power*/
void microL298Stepper::disable()
{
  digitalWrite(_coil1a, LOW);
  digitalWrite(_coil1b, LOW);
  digitalWrite(_coil2a, LOW);
  digitalWrite(_coil2b, LOW);
  analogWrite(_pwm1, 0);
  analogWrite(_pwm2, 0);
}//end disable

/*Microsteps forward or reverse*/
void microL298Stepper::takestep(boolean invert)
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

  Serial.print(_counter);
  Serial.print(" => ");
  Serial.print(_steps[_counter][0]);
  Serial.print(" ");
  Serial.print(_steps[_counter][1]);
  Serial.print(" ");
  Serial.print(_steps[_counter][2]);
  Serial.print(" ");
  Serial.print(_steps[_counter][3]);
  Serial.print(" ");
  Serial.print(_steps[_counter][4]);
  Serial.print(" ");
  Serial.print(_steps[_counter][5]);
  Serial.println();

  // use digitalWrite to turn on coil, and analogWrite to create pwm for each coil
  digitalWrite(_coil1a, _steps[_counter][0]);
  digitalWrite(_coil1b, _steps[_counter][1]);
  analogWrite(_pwm1, _steps[_counter][2]);
  digitalWrite(_coil2a, _steps[_counter][3]);
  digitalWrite(_coil2b, _steps[_counter][4]);
  analogWrite(_pwm2, _steps[_counter][5]);

}//end takestep

/*takeMicroStep forward or reverse*/
// _longCounter tracks the position.
void microL298Stepper::takeMicroStep(boolean invert)
{
  byte * p ; //Pointer to byte array of aize six.
  if (invert == false) { // CW
    _counter = _counter + 1;
    // roll over counter if they go too high
    if (_counter >= microsteps1024) {
      _counter = 0;
    }
    _longCounter++;
  }
  else { // CCW
    _counter = _counter - 1;
    // reset counter if they go too low
    if (_counter < 0) {
      _counter = microsteps1024 - 1;
    }
    _longCounter--;
  }

  //  p = _steps1024(0);
  p = _steps1024(_counter);

//Print out the step status.
//  Serial.print(_counter);
//  Serial.print("  Microstep count: ");
//  Serial.println(_longCounter);

  //  Serial.print("p= ");
  //  //  Serial.print(*p);
  //  Serial.print(int(p[0]));
  //  Serial.print(", ");
  //  Serial.print(int(p[1]));
  //  Serial.print(", ");
  //  Serial.print(int(p[2]));
  //  Serial.print(", ");
  //  Serial.print(int(p[3]));
  //  Serial.print(", ");
  //  Serial.print(int(p[4]));
  //  Serial.print(", ");
  //  Serial.print(int(p[5]));
  //  Serial.println();

  digitalWrite(_coil1a, int(p[0]));
  digitalWrite(_coil1b, int(p[1]));
  analogWrite(_pwm1, int(p[2]));
  digitalWrite(_coil2a, int(p[3]));
  digitalWrite(_coil2b, int(p[4]));
  analogWrite(_pwm2, int(p[5]));



}//end takeMicroStep
