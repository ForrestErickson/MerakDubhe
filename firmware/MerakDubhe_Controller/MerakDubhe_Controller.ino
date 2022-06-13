/*
   MerakDubhe_Controller
   A controler for the failed bench grinder becomes an equatorial mount ("turning sow's ear" into a mount)

   Hardware includes SEEED Studio Stepper Motor Control shield Revision V1.0

   Forrest Lee Erickson
   Date: 20220529
   Released into the public domain
   Usefull reference is: https://learn.adafruit.com/diy-robotic-sky-tracking-astrophotography-mount/code

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
#include "microL298Stepper.h"   //Library made by Forrest Erickson
#define BAUDRATE 115200

/*Setup external constants */
//extern long totalMicroStepsPerRevolution;
extern const int ENA ;  //Pins for H drivers A and B enable
extern const int ENB ;
extern const int IN1 ;  // pins for IN1, IN2, IN3, IN4,
extern const int IN2 ;
extern const int IN3 ;
extern const int IN4 ;

//extern String inputString;
extern bool stringComplete;

microL298Stepper rightAssentionStepper(IN1, IN2, ENA, IN3, IN4, ENB);  //Make an object of class type microL298Stepper


bool isTracking = false; //
bool isNorthTracking = true; //To Do, Save setting for this in EEPROM

//Functions here

//end functions

void setup()
{
  Serial.begin(BAUDRATE);
  //setupSerialInput();
  //  inputString.reserve(200);
  Serial.println("MerakDubhe_Controller ");
  printMotorAndGear();
  //Serial.println("End of setup.");
  commandMenu();
}//end setup()

void loop()
{

  checkCommandInput();
  updateTracking();

  // Other code

}//end loop()
