/*
   motors
   Makeing the motor(s) move.
   For: MerakDubhe_Controller
   Hardware includes SEEED Studio Stepper Motor Control shield Revision V1.0

   Forrest Lee Erickson
   Date: 20220529
   Released into the public domain
*/

/*
   Stepper motor wirring notes. Facing Seeeds board wires are left to right:
   Gray, Green, Red, Yellow.
*/

#include "microL298Stepper.h"

#define stepsPerNEMA_Revolution 200  // Motor Revolution NEMA17 motor Stepper #32A
#define microSteps_PerRevolution 51200 // for this NEMA17 shaft = 200*256
#define RA_GEER_to_NEMA_GEER 20   //Gears of MerakDubhe, about 10 Inch over 1/2 inch. 
#define stepsPerShaftRevolution 4000// = stepsPerNEMA_Revolution * RA_GEER_to_NEMA_GEER; // 200*20=4000 Steps per RA gear revolution
#define totalMicroStepsPerRevolution 1024000 // = 200*256L*20; // 200*256*20 = 1024000 microSteps per RA gear revolution

//float secondsPerSideralDay = 86164.1F;
//unsigned long mSecondsPerSideralDay = secondsPerSideralDay * 1000;
#define mSecondsPerSideralDay 86164100

//float microStepPeriod = mSecondsPerSideralDay/totalMicroStepsPerRevolution;// Should be 84.14462890625
float microStepPeriod = 84.14462890625;   // Should be 84.14462890625
float nextSideralStep = 0;                // Acumulate sideral time for steps.

void updateTracking() {
  if (isTracking) {
    if (millis() > nextSideralStep) {
      //Make a step, Set next step time
      rightAssentionStepper.takeMicroStep(isNorthTracking);
      nextSideralStep = nextSideralStep + microStepPeriod;
    }//end next step
  }//end is tracking


}//end updateTracking


void printMotorAndGear() {
  Serial.print("System stepsPerNEMA_Revolution: ");
  Serial.println(stepsPerNEMA_Revolution);
  Serial.print("System microSteps_PerRevolution: ");
  Serial.println(microSteps_PerRevolution);
  Serial.print("System RA_GEER_to_NEMA_GEER: ");
  Serial.println(RA_GEER_to_NEMA_GEER);
  Serial.print("System stepsPerShaftRevolution: ");
  Serial.println(stepsPerShaftRevolution);
  Serial.print("System totalMicroStepsPerRevolution: ");
  Serial.println(totalMicroStepsPerRevolution);
  Serial.print("System mSecondsPerSideralDay: ");
  Serial.println(mSecondsPerSideralDay);
  Serial.print("System microStepPeriod: ");
  Serial.println(microStepPeriod, 6);

}// end printMotorAndGear

// Define some steppers and the pins they will use for Seeeds Studio Motor Shield V1.0
const int ENA = 9;  //Pins for H drivers A and B enable
const int ENB = 10;  //These pins can be Arduino UNO AnalogWrite pwm at 490 Hz..
// pins for IN1, IN2, IN3, IN4,
const int IN1 = 8;  //Non-pwm
const int IN2 = 11;  //pwm
const int IN3 = 12;
const int IN4 = 13;


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


/*
  Move the motor back and forth just a bit. Attach the queen if you so desire.
  This function blocks.
  Argument is step delay in mSec.
*/
void wave(int stepDelay) {
  const  int NUMBER_OF_STEPS_WAVE = stepsPerShaftRevolution / 12;
  int _stepDelay = stepDelay;
  //Motor back and forth.
  Serial.println("clockwise");
  for (int i = 0; i < NUMBER_OF_STEPS_WAVE; i++) {
    Serial.print(i);
    rightAssentionStepper.takestep(true);
    delay(_stepDelay);
  }//end CW
  delay(100);
  Serial.println("counterclockwise");
  for (int i = 0; i < NUMBER_OF_STEPS_WAVE; i++) {
    rightAssentionStepper.takestep(false);
    Serial.print(-i);
    delay(_stepDelay);
  }//end CCW
  rightAssentionStepper.disable();    //Save power.
}//end wave()


/*
  Usint 255 micro steps
  Move the motor back and forth just a bit. Attach the queen if you so desire.
  This function blocks.
  Argument is step delay in mSec.
*/
void microWave(int stepDelay) {
  //  const  int NUMBER_OF_STEPS_WAVE = 1600;
  const  long NUMBER_OF_USTEPS_WAVE = totalMicroStepsPerRevolution / 12;
  int _stepDelay = stepDelay;
  Serial.println("Motor back and forth.");
  Serial.println("clockwise");
  for (int i = 0; i < NUMBER_OF_USTEPS_WAVE; i++) {
    Serial.print(i);
    rightAssentionStepper.takeMicroStep(true);
    delay(_stepDelay);
  }//end CW
  delay(1000);
  Serial.println("counterclockwise");
  for (int i = 0; i < NUMBER_OF_USTEPS_WAVE; i++) {
    rightAssentionStepper.takeMicroStep(false);
    Serial.print(-i);
    delay(_stepDelay);
  }//end CCW
  rightAssentionStepper.disable();    //Save power.
}//end wave()
