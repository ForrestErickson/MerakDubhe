/*
 * Transformed for L298
 * By Forrest Lee Erickson
 * Date: 20220601
 * 
 * Found at: https://forum.arduino.cc/t/h-bridge-microstepping-and-only-two-pwms/461671

  microStepper4.h - Library for stepper motor driving
  Created by San Zamoyski, 30.05.2017 

	based on

  Created by Clayton P. Webster, February 20th, 2010
  Released into the public domain
*/

#ifndef microL298Stepper_h // keep from including twice
#define microL298Stepper_h

#define microsteps 16

#include "Arduino.h"

class microStepper8
{
  public:
    microStepper8(int coil1a, int coil1b, int coil2a, int coil2b);
    void disable();
    void takestep(boolean invert);
    
  private:
    int _coil1a;
    int _coil1b;
    int _coil2a;
    int _coil2b;
    
    int _counter;
    boolean _invert;
    //const int microsteps = 16;

	//8 steps: uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
	//4 steps: uint8_t microstepcurve[] = {0, 98, 180, 236, 255};	//0
	//4 steps: uint8_t microstepcurve[] = {255, 157, 75, 19, 0};	//1
	byte _steps[microsteps][4] = {

        //1/4 step
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
	};
};

#endif
