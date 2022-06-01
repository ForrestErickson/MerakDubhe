/*
   motors
   For: MerakDubhe_Controller
   Based on SEEED Studio Stepper Motor Control - one revolution
   example usesf the <Stepper.h> library

   Forrest Lee Erickson
   Date: 20220529

*/

/*
 * Stepper motor wirring notes. Facing Seeeds board wires are left to right:
 * Gray, Green, Red, Yellow.
 */

const int stepsPerNEMA_Revolution = 200;  // Motor Revolution NEMA17 motor Stepper #32A
const int RA_GEER_to_NEMA_GEER = 20;  //10 Inch over 1/2 inch.
int stepsPerRevolution = stepsPerNEMA_Revolution * RA_GEER_to_NEMA_GEER; // Steps per RA gear revolution

// Define some steppers and the pins they will use
// initialize the Stepper object for Right Assention
const int ENA = 9;  //Pins for H drivers A and B enable
const int ENB = 10;
const int IN1 = 8;  // pins for IN1, IN2, IN3, IN4,
const int IN2 = 11;
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



void wave() {
  //Motor back and forth.
  Serial.println("clockwise");
  rightAssentionStepper.step(stepsPerRevolution / 10);
  delay(100);
  Serial.println("counterclockwise");
  rightAssentionStepper.step(-stepsPerRevolution / 10);
}//end wave()
