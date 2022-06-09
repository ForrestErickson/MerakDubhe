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
#define microsteps1024 1024   //Number of micro steps. Constant to be used for array allocation. Four phases and 256 steps.

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
    long counter(); // Returns _counter
    int *steps1024(int count);

  private:
    int _coil1a;
    int _coil1b;
    int _coil2a;
    int _coil2b;
    int _pwm1;
    int _pwm2;
    long _counter;
    boolean _invert;

    //?16? steps: uint8_t microstepcurve[] = {0, 19, 75, 157, 255};
    //8 steps: uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
    //4 steps: uint8_t microstepcurve[] = {0, 98, 180, 236, 255};	//0
    //4 steps: uint8_t microstepcurve[] = {255, 157, 75, 19, 0};	//1

    byte _steps[microsteps][6] = {
      //New 20220608 size argument step pattern
      //(int coil1a, int coil1b, int pwm1, int coil2a, int coil2b, int pwm2)
      //Micro step is 1/4 of a full step. For 200 step motor this gives 800 micro steps for a revolution.
      //phase1
      //1a, 1b, pwm1, 2a, 2b, pwm2)
      {1, 0, 255, 1, 0, 0},
      {1, 0, 192, 1, 0, 64},
      {1, 0, 128, 1, 0, 128},
      {1, 0, 64, 1, 0, 192},
      //phase2
      {0, 1, 0, 1, 0, 255},
      {0, 1, 64, 1, 0, 192},
      {0, 1, 128, 1, 0, 128},
      {0, 1, 192, 1, 0, 64},
      //phase3
      {0, 1, 255, 0, 1, 0},
      {0, 1, 192, 0, 1, 64},
      {0, 1, 128, 0, 1, 128},
      {0, 1, 64, 0, 1, 192},
      //phase4
      {1, 0, 0, 0, 1, 255},
      {1, 0, 64, 0, 1, 192},
      {1, 0, 128, 0, 1, 128},
      {1, 0, 192, 0, 1, 64}
    };//end _steps

    /*Returns pointer to an array for the value of count*/
    int * _steps1024(int count) {
      //byte _steps1024[int count][6] = {
      //(int coil1a, int coil1b, int pwm1, int coil2a, int coil2b, int pwm2)

      //1a, 1b, pwm1, 2a, 2b, pwm2)
      static int r[6];

      if (count < 256) {        //phase1
        r[0] = 1; r[1] = 0; r[2] = (255 - count); r[3] = 1; r[4] = 0; r[5] = count;
        //  Serial.println(1, 0, (256 - count), 1, 0, count);
        
        //Print it out
        for ( int i = 0; i < 6; ++i) {
          //Serial.println( "r[%d] = %d\n", i, r[i]);
          Serial.print( "r[");
          Serial.print( i);
          Serial.print( "] = ");
          Serial.print( int(r[i]));
          Serial.print(", ");
//          Serial.print( "r[%d] = %d\n", i, r[i]);
        }
//        Serial.println(1, 0, (256 - count), 1, 0, count);
        return (r);
      }
      
//      else if (count < 512) {        //phase2
//        return (0, 1, (count - 512), 1, 0, 512 - count);
//      } else if (count < 768) {      //phase4
//        return (0, 1, 768 - count, 0, 1, count - 768));
//      } else { // count between 767 and 1024       //phase4
//        return (1, 0, count - 1024, 0, 1, 1024 - (count));
//      }
    }// end steps1024

    //return (*_steps1024);

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
