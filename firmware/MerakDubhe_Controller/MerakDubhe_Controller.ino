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
 #include <Stepper.h>

const int stepsPerNEMA_Revolution = 200;  // Motor Revolution NEMA17 motor Stepper #32A
const int RA_GEER_to_NEMA_GEER = 20;  //10 Inch over 1/2 inch.
const int stepsPerRevolution = stepsPerNEMA_Revolution * RA_GEER_to_NEMA_GEER; // Steps per RA gear revolution

// Define some steppers and the pins they will use
// initialize the Stepper object for Right Assention
const int ENA = 9;  //Pins for H drivers A and B enable
const int ENB = 10;
const int IN1 = 8;  // pins for IN1, IN2, IN3, IN4,
const int IN2 = 11;
const int IN3 = 12;
const int IN4 = 13;

Stepper rightAssentionStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);


const int HIGH_TIME_LED = 5000;
const int LOW_TIME_LED = 5000;
unsigned long lastLEDtime = 0;
unsigned long nextLEDchange = 100; //time in ms.


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
  rightAssentionStepper.setSpeed(6);
  wave();  //Motor back and forth.
  disableRA_Stepper();

  // Your application setup code here
  isMotorCW = true; //
  isAdvancing = true; //

}//end setup()

void loop()
{

  // wave();  //Motor back and forth.

  // Other code

}//end loop()
