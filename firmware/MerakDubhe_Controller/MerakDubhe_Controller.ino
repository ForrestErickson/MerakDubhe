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
  D4              Button to ground.
  D5~             Tracking_LED to power.
  D6~              /Focus    ?ring?
  D7               /Shutter  ?tip?
  D8               MOTOR CONTROL
  D9~              Output Enable Bridge A, M1
  D10~             Output Enable Bridge B, M2
  D11~             MOTOR CONTROL
  D12              MOTOR CONTROL
  D13              MOTOR CONTROL
*/

#include <DailyStruggleButton.h>
//#include <CameraControl>

// Create an instance of DailyStruggleButton
DailyStruggleButton myButton;
extern unsigned int longPressTime;
extern byte multiHitTarget;
extern unsigned int multiHitTime;

// include the library
#include "microL298Stepper.h"   //Library made by Forrest Erickson
#define BAUDRATE 115200

#define NOT_RELEASE_MOTOR_BUTTON 4  //Use pin D4 pullup for a button to GND that will release the motor and stop tracking.

#define nTracking_LED 5 // Tracking LED from power.

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

bool isTimeLaps = false; //Start out not making photographs
bool isLastExposure = false; 

/*Wired Camera Control for Focus and Shutter
   By Forrest Lee Erickson
   Date: 20220701
   License: Released into the public domain, dedicated to the public domain
   Warrentee: This code is designed to kill you and render the earth uninhabitable but is not guarenteed to do so.

   Developed for Canon T3 camera and tested with the same.  See my Railduino project too.
*/

class WiredCamera {
    /*  Wiring from Railduino
        D6~              /Focus    ?ring?
        D7               /Shutter  ?tip?
    */
    //    const  int EXPOSURE_TIME = 30000;  // 30 seconds for time laps
    const  int EXPOSURE_TIME = 1000;  // 1 second for development.
    const  int nFOCUS = 6;  // Pin assignment. Make low to trigger auto focus.
    const  int nSHUTTER = 7;  // Pin assignment. Make low to trigger open shutter.
    const  int  FOCUS_DELAY = 1000;  //mSec delay from focus to shutter release.
    //long OffTime;    // milliseconds of off-time
    const long PHOTO2SD = 3000;    // milliseconds of time to load photo into SD card

    int _focusPin = nFOCUS;
    int _shutterPin = nSHUTTER;
    int _focusDelay = FOCUS_DELAY;
    int _exposureTime = EXPOSURE_TIME;
    unsigned long previousMillis;    // will store last time LED was
    bool _isShutterOpen = false;

  public:
    //Set up the pins for this instance.
    WiredCamera(int focus_Pin, int shutter_Pin)
    {
      _focusPin = focus_Pin;
      _shutterPin = shutter_Pin;
      _focusDelay = FOCUS_DELAY;
      _isShutterOpen = false;
    }

    void printCameraPins() {
      Serial.print("Focus pin: ");
      Serial.println(_focusPin);
      Serial.print("Shutter pin: ");
      Serial.println(_shutterPin);
    }//end printCameraPins

    /*
       Focus and Shutter pins are high impedance at power up. Set for input just to make sure.
       Write the ditital output as LOW, but since input the camera is not changed.
       To activate foucus or shurter release write pinMode from input to output and then back to input.
    */

    void setupCameraWiredInterface() {
      pinMode(_focusPin, INPUT);  //Set as input for high impedance.
      pinMode(_shutterPin, INPUT); // Set as input for high impedance.
      digitalWrite(_focusPin, LOW);  // Set for pin low to sink current when low impedance.
      digitalWrite(_shutterPin, LOW);
      //delay(50);
    }//end setupCameraWiredInterface

    void setExposureTimeSeconds(int seconds) {
      _exposureTime = 1000 * seconds; //milliseconds for expsoure time.
      Serial.print("ExposureTime:  ");
      Serial.println(seconds);
    }//end setupCameraWiredInterface

    void setAutoFocus() {
      // Trigger auto focus before photo
      pinMode(_focusPin, OUTPUT);  // Make low impedance
      Serial.print("Focusing! ");
      delay(_focusDelay);
      pinMode(_focusPin, INPUT);    //Make high impedance.
      Serial.println("Focus set");
    }//end setAutoFocus()

    void focusAndPhoto() {
      // Trigger auto focus before photo
      pinMode(_focusPin, OUTPUT);  // Make low impedance
      Serial.print("Focusing! ");
      delay(_focusDelay);
      pinMode(_focusPin, INPUT);    //Make high impedance.
      Serial.println("Focus set");

      //Shutter release
      pinMode(_shutterPin, OUTPUT);  // Make low impedance
      Serial.print("Shutter release ");
      //delay(_focusDelay);
      delay(1);
      pinMode(_shutterPin, INPUT);    //Make high impedance.
    }//end focusAndPhoto()

    void makePhoto() {
      //Shutter release
      pinMode(_shutterPin, OUTPUT);  // Make low impedance
      Serial.println("Shutter release ");
      //delay(_focusDelay);
      delay(500);
      pinMode(_shutterPin, INPUT);    //Make high impedance.
    }//end makePhoto()

    //Make photo of time _exposureTime. Updates _isShutterOpen
    void updateTimeLaps() {
      unsigned long currentMillis = millis();
      if (isTimeLaps) {
        if ((currentMillis - previousMillis >= PHOTO2SD) && !_isShutterOpen) {
          pinMode(_shutterPin, OUTPUT);  // Make low impedance
          Serial.println("Shutter open ");
          previousMillis = previousMillis + PHOTO2SD;  // Update the time
          _isShutterOpen = true;
        }//end time has elapsed
        else if ((currentMillis - previousMillis >= _exposureTime) && _isShutterOpen) {
          if (isLastExposure){
            isTimeLaps = false;
            isLastExposure = false;
          }//Only stop time laps at end of exposure.
          pinMode(_shutterPin, INPUT);    //Make high impedance.
          Serial.println("Shutter closed ");
          previousMillis = previousMillis + _exposureTime;  // Update the time
          _isShutterOpen = false;
        }

      }//end isTimeLaps
    }//end updateTimeLaps

};//end WiredCamera class


WiredCamera myCanonT3(6, 7) ; //Make a camera object


bool isTracking = true; //  On 20220625 canged to true so that tracking starts at power up.
bool isNorthTracking = true; //To Do, Save setting for this in EEPROM

//Functions here
//Function to handle polarity of LED.
void TrackingLED (bool stateLED) {
  bool _stateLED = stateLED;
  if (_stateLED) {
    digitalWrite(nTracking_LED, LOW);
  } else {
    digitalWrite(nTracking_LED, HIGH);
  }//end else

}// end TrackingLED

//end functions

void setup()
{
  delay(100);
  Serial.begin(BAUDRATE);
  delay(100);

  myCanonT3.setupCameraWiredInterface();
  myCanonT3.printCameraPins();
  //  myCanonT3.focusAndPhoto();
  //  myCanonT3.setAutoFocus();
  //  setupCameraWiredInterface();

  //Turn on tracking LED
  digitalWrite(nTracking_LED, LOW);
  pinMode (nTracking_LED, OUTPUT);

  myButton.set(NOT_RELEASE_MOTOR_BUTTON, buttonEvent, INT_PULL_UP);
  // You can enable long press to use this feature
  myButton.enableLongPress(longPressTime);
  // You can enable multi-hit to use this feature
  myButton.enableMultiHit(multiHitTime, multiHitTarget);

  //  Serial.print("TCCR1B= "); //Print default TCCR1B
  //  Serial.println(TCCR1B);
  //  TCCR1B = TCCR1B & B11111000 | B00000011; // Set for  clkI/O/64 (From prescaler
  //Speed up PWM above audio so motor does not sing.
  TCCR1B = TCCR1B & B11111000 | B00000001; // PWM 62745.10 Hz pins 9 and 10, No prescaling,  clkI/O/1.
  //  Serial.print("TCCR1B= ");
  //  Serial.println(TCCR1B);

  // Lock the RA gear at start up.
  rightAssentionStepper.takeMicroStep(isNorthTracking); //Take a step to energize motor
  isTracking = false; //Then turn off tracking.
  TrackingLED(false);

  commandMenu();
}//end setup()

void loop()
{
  myButton.poll();

  checkCommandInput();

  updateTracking();

  myCanonT3.updateTimeLaps();

  // Other code

}//end loop()
