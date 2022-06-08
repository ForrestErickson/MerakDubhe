/*
   Transformed for L298
   By Forrest Lee Erickson
   Date: 20220601

   Found at: https://forum.arduino.cc/t/h-bridge-microstepping-and-only-two-pwms/461671

  microStepper4.h - Library for stepper motor driving
  Created by San Zamoyski, 30.05.2017
	based on
  Created by Clayton P. Webster, February 20th, 2010
  Released into the public domain
*/

#ifndef microL298Stepper_h // keep from including twice
#define microL298Stepper_h

//This increases the steps by the number of micro steps per phase which is 4.
#define microsteps 16   //Number of micro steps. Constant to be used for array allocation. Four phases and four steps.

#include "Arduino.h"

//A class for the Seeeds Studio Motor Shield V1.0 Arduino sheild.
//Pin pattern:  non-pwm non-pwm pwm   non-pwm   non-pwm  pwm
class microL298Stepper
{
  public:
    microL298Stepper(int coil1a, int coil1b, int pwm1, int coil2a, int coil2b, int pwm2);
    void hold();                                //Energise coils for electronic break. Opposit of disable.
    void disable();                             //Releases electronic break.
    void takestep(boolean invert);
    int counter(); // Returns _counter

  private:
    int _coil1a;
    int _coil1b;
    int _coil2a;
    int _coil2b;
    int _pwm1;
    int _pwm2;
    int _counter;
    boolean _invert;

    //?16? steps: uint8_t microstepcurve[] = {0, 19, 75, 157, 255};
    //8 steps: uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
    //4 steps: uint8_t microstepcurve[] = {0, 98, 180, 236, 255};	//0
    //4 steps: uint8_t microstepcurve[] = {255, 157, 75, 19, 0};	//1
    byte _steps[microsteps][6] = {

      //New 20220601 size argument step pattern
      //(int coil1a, int coil1b, int pwm1, int coil2a, int coil2b, int pwm2)
      //Micro step is 1/4 of a full step. For 200 step motor this gives 800 micro steps for a revolution.
      //phase1
      {1,   0,   255, 1,   0, 0},
      {1,   0,   157, 1,   0, 19},
      {1,   0,    75, 1,   0, 75},
      {1,   0,   19,  1,   0, 157},

      //phase2
      {0,   1,   0,    1,   0, 255},
      {0,   1,   19,   1,   0, 157},
      {0,   1,   75,   1,   0, 75},
      {0,   1,   157,  1,   0, 19},

      //phase3
      {0,   1,   255,  0,   1, 0},
      {0,   1,   157,  0,   1, 19},
      {0,   1,    75,  0,   1, 75},
      {0,   1,    19,  0,   1, 157},

      //phase4
      {1,   0,    0,  0,   1, 255},
      {1,   0,    19,  0,   1, 157},
      {1,   0,    75,  0,   1, 75},
      {1,   0,    157,  0,   1, 19}
    };//end _steps
};//end class microL298Stepper


//Old step paterns from library as copyed
//Old four argument step pattern
//1/4 step
/*
   {0,   0,   1,   0},
    {1,   157,   1,   19},
    {1,   75,   1,   75},
    {1,   19,   1,   157},

    {1,   0,   1,   255},
    {1, 19,   0,   98},
    {1, 75,   0,   180},
    {1, 157,   0,   236},

    {1, 255,   0,   255},
    {0, 98,   0,   236},
    {0, 180,   0, 180},
    {0, 236,   0, 98},

    {0,   255,   0, 0},
    {0,   236,   1, 157},
    {0,   180,   1, 75},
    {0,   0,   1, 19}
*/

//halfstep
/*{1,   0,   0,   0},
  {1,   0,   0,   0},
  {1,   0,   1,   0},
  {1,   0,   1,   0},

  {0,   0,   1,   0},
  {0,   0,   1,   0},
  {0, 255,   1,   0},
  {0, 255,   1,   0},

  {0, 255,   0,   0},
  {0, 255,   0,   0},
  {0, 255,   0, 255},
  {0, 255,   0, 255},

  {0,   0,   0, 255},
  {0,   0,   0, 255},
  {1,   0,   0, 255},
  {1,   0,   0, 255}*/

//full step
/*{1,   0,   0,   0},
  {1,   0,   0,   0},
  {1,   0,   0,   0},
  {1,   0,   0,   0},

  {0,   0,   1,   0},
  {0,   0,   1,   0},
  {0,   0,   1,   0},
  {0,   0,   1,   0},

  {0, 255,   0,   0},
  {0, 255,   0,   0},
  {0, 255,   0,   0},
  {0, 255,   0,   0},

  {0,   0,   0, 255},
  {0,   0,   0, 255},
  {0,   0,   0, 255},
  {0,   0,   0, 255}*/

//Pattern ???
/*{1,   0,   1, 255}, //!
  {1, 110,   1, 215},
  {1, 170,   1, 170},
  {1, 245,   1,  60},

  {0,   0,   1,   0}, //!
  {0,  30,   1,  60},
  {0,  85,   1, 170},
  {0, 155,   1, 225},

  {0, 255,   0,   0}, //!
  {0, 255,   0,   0},
  {0, 255,   0,   0},
  {0, 255,   0,   0},

  {1, 255,   0, 255}, //!
  {0,   0,   0, 255},
  {0,   0,   0, 255},
  {0,   0,   0, 255}*/

/*{1,   0,   0,   0},
  {1,   0,   0,  50},
  {1,   0,   0, 120},
  {1,   0,   0, 255},

  {1, 135,   0, 255},
  {1, 205,   0, 255},
  {1, 255,   0, 255},
  {0,  50,   0, 255},

  {0, 120,   0, 255},
  {0, 255,   0, 255},
  {0, 255,   0, 120},
  {0, 255,   0,  50},

  {0,  75,   1,   0},
  {0,  19,   1,   0},
  {0,   0,   1,   0},
  {0, 255,   1, 255},
  {0, 255,   0,  98},
  {0, 255,   0, 180},
  {0, 170,   0, 236},
  {0,  85,   0, 255},
  {1,  85,   0, 255},
  {1, 170,   0, 255},
  {1,  85,   0, 255},
  {1,   0,   0,  85}*/


#endif
