/*Wired Camera Control for Focus and Shutter
   By Forrest Lee Erickson
   Date: 20220701
   License: Released into the public domain, dedicated to the public domain
   Warrentee: This code is designed to kill you and render the earth uninhabitable but is not guarenteed to do so.

   Developed for Canon T3 camera and tested with the same.  See my Railduino project too.
*/

#include "Arduino.h"
#include "cameracontrol.h"

/*Wired Camera Control for Focus and Shutter
   By Forrest Lee Erickson
   Date: 20220701
   License: Released into the public domain, dedicated to the public domain
   Warrentee: This code is designed to kill you and render the earth uninhabitable but is not guarenteed to do so.

   Developed for Canon T3 camera and tested with the same.  See my Railduino project too.
*/


//Set up the pins for this instance.
WiredCamera::WiredCamera(int focus_Pin, int shutter_Pin) {
  _focusPin = focus_Pin;
  _shutterPin = shutter_Pin;
}//end WiredCamera

void WiredCamera::printCameraPins() {
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
void WiredCamera::setupCameraWiredInterface() {
  pinMode(_focusPin, INPUT);  //Set as input for high impedance.
  pinMode(_shutterPin, INPUT); // Set as input for high impedance.
  digitalWrite(_focusPin, LOW);  // Set for pin low to sink current when low impedance.
  digitalWrite(_shutterPin, LOW);
  //delay(50);
}//end setupCameraWiredInterface

void WiredCamera::setExposureTimeSeconds(long seconds) {
  _exposureTime = 1000 * seconds; //milliseconds for expsoure time.
  Serial.print("Set ExposureTime:  ");
  Serial.println(_exposureTime/1000);
}//end setExposureTimeSeconds

void WiredCamera::printExposureTime() {
//  _exposureTime = 1000 * seconds; //milliseconds for expsoure time.
  Serial.print("ExposureTime:  ");
  Serial.println(_exposureTime/1000);
}//end setExposureTimeSeconds

void WiredCamera::setAutoFocus() {
  // Trigger auto focus before photo
  pinMode(_focusPin, OUTPUT);  // Make low impedance
  Serial.print("Focusing! ");
  delay(_focusDelay);
  pinMode(_focusPin, INPUT);    //Make high impedance.
  Serial.println("Focus set");
}//end setAutoFocus()

void WiredCamera::focusAndPhoto() {
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

void WiredCamera::makePhoto() {
  //Shutter release
  pinMode(_shutterPin, OUTPUT);  // Make low impedance
  Serial.println("Shutter release ");
  //delay(_focusDelay);
  delay(500);
  pinMode(_shutterPin, INPUT);    //Make high impedance.
}//end makePhoto()

void WiredCamera::setLastExposure(bool isLastExposure) {
  // Make local variable
  _isLastExposure = isLastExposure;
}//end setLastExposure()

//Make photo of time _exposureTime. Updates _isShutterOpen returns isTimeLaps
//Need to have this setup ready to go when isTimeLaps becomes true.
bool WiredCamera::updateTimeLaps() {
  unsigned long currentMillis = millis();
    if ((currentMillis - _previousMillis >= PHOTO2SD) && !_isShutterOpen) {//Card written so make photo.
      pinMode(_shutterPin, OUTPUT);  // Make low impedance
      Serial.println("Shutter open ");
      _previousMillis = _previousMillis + PHOTO2SD;  // Update the time
      _isShutterOpen = true;
      return(true); //Shutter not open so we are writing to card.
    }//end time has elapsed
    else if ((currentMillis - _previousMillis >= _exposureTime) && _isShutterOpen) {
      pinMode(_shutterPin, INPUT);    //Make high impedance to close.
      Serial.println("Shutter closed ");
      _previousMillis = _previousMillis + _exposureTime;  // Update the time
      _isShutterOpen = false;
      if (_isLastExposure) {
        _isLastExposure = false;
        return(false);//Return is time laps
      }else {
        return(true); //Not yet at last exposure
      }//end else if >exposure time and shutter open      
  }//end isTimeLaps
}//end updateTimeLaps

// };//end WiredCamera class
