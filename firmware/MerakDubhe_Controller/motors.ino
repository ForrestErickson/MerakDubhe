/*
   motors
   For: MerakDubhe_Controller
   Based on SEEED Studio Stepper Motor Control - one revolution
   example usesf the <Stepper.h> library

   Forrest Lee Erickson
   Date: 20220529

*/

/*
   Stepper motor wirring notes. Facing Seeeds board wires are left to right:
   Gray, Green, Red, Yellow.
*/

#include "microL298Stepper.h"

const int stepsPerNEMA_Revolution = 200;  // Motor Revolution NEMA17 motor Stepper #32A
const int RA_GEER_to_NEMA_GEER = 20;  //10 Inch over 1/2 inch.
int stepsPerRevolution = stepsPerNEMA_Revolution * RA_GEER_to_NEMA_GEER; // Steps per RA gear revolution

// Define some steppers and the pins they will use
// initialize the Stepper object for Right Assention
//These worked with Stepper library for full steps.
//const int ENA = 9;  //Pins for H drivers A and B enable
//const int ENB = 10;
//const int IN1 = 8;  // pins for IN1, IN2, IN3, IN4,
//const int IN2 = 11;
//const int IN3 = 12;
//const int IN4 = 13;

//non-pwm pwm   non-pwm   pwm
//microStepper8::microStepper8(int coil1a, int coil1b, int coil2a, int coil2b)
const int ENA = 9;  //Pins for H drivers A and B enable
const int ENB = 10;
// pins for IN1, IN2, IN3, IN4,
const int IN1 = 8;  //Non-pwm
const int IN2 = 11;  //pwm
const int IN3 = 12;
const int IN4 = 13;



//Stepper rightAssentionStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

void enableRA_Stepper() {
  pinMode(ENA, OUTPUT); //EnA
  pinMode(ENB, OUTPUT);  //EnB
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}//end enableRA_Stepper()

void disableRA_Stepper() {
  //saves power
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}//end enableRA_Stepper()


/*Move the motor back and forth just a bit. Attach the queen if you so desire.*/

void wave() {
  const  int NUMBER_OF_STEPS_WAVE = 100;
  //Motor back and forth.
  Serial.println("clockwise");
  for (int i = 0; i < NUMBER_OF_STEPS_WAVE; i++) {
    Serial.print(i);
    Serial.print(": ");
    rightAssentionStepper.takestep(true);
    delay(100);
  }

  //  microStepper8->takestep(true);
  //  rightAssentionStepper.step(stepsPerRevolution / 10);
  delay(100);
  Serial.println("counterclockwise");
  for (int i = 0; i < NUMBER_OF_STEPS_WAVE; i++) {
    rightAssentionStepper.takestep(false);
    Serial.print(i);
    Serial.print(": ");
    delay(100);
  }
  //  microStepper8->takestep(false);
  //rightAssentionStepper.step(-stepsPerRevolution / 10);
}//end wave()
