/*
   MerakDubhe_Controller
   A controler for the failed bench grinder becomes an equatorial mount ("turning sow's ear" into a mount)

   Based on SEEED Studio Stepper Motor Control - one revolution
   example usesf the <Stepper.h> library

   Forrest Lee Erickson
   Date: 20220529
*/

/*
  Pin assignments
  A0               Auto / Serial Control  High if automatic advance.
  A1 - A5          NC

  D0  RX
  D1  TX
  D2
  D3~
  D4
  D5~
  D6~              /Focus    ?ring?
  D7               /Shutter  ?tip?
  D8               MOTOR CONTROL
  D9~              Output Enable Bridge A, M1
  D10~             Output Enable Bridge B, M2
  D11~             MOTOR CONTROL
  D12              MOTOR CONTROL
  D13              MOTOR CONTROL
*/

// include the library
// #include <Stepper.h>

#include "microL298Stepper.h"


extern int stepsPerRevolution;
extern const int ENA ;  //Pins for H drivers A and B enable
extern const int ENB ;
extern const int IN1 ;  // pins for IN1, IN2, IN3, IN4,
extern const int IN2 ;
extern const int IN3 ;
extern const int IN4 ;


//microStepper8 rightAssentionStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

//microL298Stepper rightAssentionStepper(IN1, IN2, IN3, IN4);  //Make an object of type microStepper8
microL298Stepper rightAssentionStepper(IN1, IN2, ENA, IN3, IN4, ENB);  //Make an object of type microStepper8

//Stepper rightAssentionStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);
//extern Stepper rightAssentionStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);
//extern Stepper rightAssentionStepper();


bool isMotorCW = true; //
bool isAdvancing = true; //

//Functions here


//end functions

void setup()
{
  Serial.begin(112500);
  Serial.print("System stepsPerRevolution: ");
  Serial.println(stepsPerRevolution);

  // Enable H drivers A and B
    enableRA_Stepper();

  //  rightAssentionStepper.setSpeed(6);
  int stepCount = 10;
  while (stepCount >0) {
    wave();  //Motor back and forth.
    stepCount--;
  }

  //  disableRA_Stepper();

  //  rightAssentionStepper.disable();

  //  disableRA_Stepper();

  // Your application setup code here
  isMotorCW = true; //
  isAdvancing = true; //

}//end setup()

void loop()
{

  // wave();  //Motor back and forth.

  // Other code

}//end loop()
