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
    void takeMicroStep(boolean invert);
    long longCounter(); // Returns _longCounter
    int counter(); // Returns _counter
    byte *steps1024(int count);

  private:
    int _coil1a;
    int _coil1b;
    int _coil2a;
    int _coil2b;
    int _pwm1;
    int _pwm2;
    int _counter;
    long _longCounter = 0;
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
    //(int coil1a, int coil1b, int pwm1, int coil2a, int coil2b, int pwm2)
    //1a, 1b, pwm1, 2a, 2b, pwm2)
    byte * _steps1024(int count) {
      static byte r[6];

      if (count < 256) {        //phase1
        r[0] = 1; r[1] = 0; r[2] = (255 - count); r[3] = 1; r[4] = 0; r[5] = count;
      } else if (count < 512) {        //phase2
        r[0] = 0; r[1] = 1; r[2] = (count - 256); r[3] = 1; r[4] = 0; r[5] = (511 - count);
      } else if (count < 768) {      //phase3
        r[0] = 0; r[1] = 1; r[2] = (767 - count); r[3] = 0; r[4] = 1; r[5] = (count - 512);
      } else { // count between 767 and 1024       //phase4
        r[0] = 1; r[1] = 0; r[2] = (count - 768); r[3] = 0; r[4] = 1; r[5] = (1023 - count);
      }//end calcaulation from count

      //      //Print it out
      //      for ( int i = 0; i < 6; ++i) {
      //        //Serial.println( "r[%d] = %d\n", i, r[i]);
      //        Serial.print( "r[");
      //        Serial.print( i);
      //        Serial.print( "] = ");
      //        Serial.print( int(r[i]));
      //        Serial.print(", ");
      //      }//end print it out

      return (r);// The coils and PWM patterns for 256 steps on 4 phases
    }// end steps1024

    //return (*_steps1024);

};//end class microL298Stepper
#endif
