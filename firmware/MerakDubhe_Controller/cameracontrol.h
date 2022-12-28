/*
  cameracontrol.h - Library for controling a camera focus and shutter using an open drain on Aruino.
  Created by Forrest Lee Erickson
  Date: 20220702
  Released into the public domain.
*/

// keep from including twice
#ifndef cameracontrol_h
#define cameracontrol_h

#include "Arduino.h"

class WiredCamera {

  public:
    /*  Wiring from Railduino
      D6~              /Focus    ?ring?
      D7               /Shutter  ?tip?
    */
    const  unsigned long EXPOSURE_TIME = 70000;  // 70 seconds for max exposure, Noise cancelation and some overhead
    //const  unsigned long EXPOSURE_TIME = 30000;  // 30 seconds for time laps
//    const  unsigned long EXPOSURE_TIME = 1000;  // 1 second for development.
    const  int nFOCUS = 6;  // Pin assignment. Make low to trigger auto focus.
    const  int nSHUTTER = 7;  // Pin assignment. Make low to trigger open shutter.
    const  int  FOCUS_DELAY = 1000;  //mSec delay from focus to shutter release.
    //long unsigned OffTime;    // milliseconds of off-time
    const unsigned long PHOTO2SD = 3000;    // milliseconds of time to load photo into SD card


    //Set up the pins for this instance.
    WiredCamera(int focus_Pin, int shutter_Pin);

    void printCameraPins();
    void setupCameraWiredInterface();
    void setAutoFocus();
    void focusAndPhoto();
    void makePhoto();
    bool updateTimeLaps();
    void setExposureTimeSeconds(unsigned long seconds);
    void printExposureTime();
    void printExposureCount();
    void setLastExposure(bool isLastExposure);
    void restartTimeLaps();

  private:
    //    int _focusPin = focus_Pin;
    //    int _shutterPin = shutter_Pin;
    int _focusPin = nFOCUS;
    int _shutterPin = nSHUTTER;
    int _focusDelay = FOCUS_DELAY;
    unsigned long _exposureTime = EXPOSURE_TIME;
    unsigned long _previousMillis = 0;    // will store last time LED was
    unsigned long _exposureCount=0;
    bool _isShutterOpen = false;
    bool _isLastExposure = false; //Lets start able to make expsoures.

}; //end of class decleration



#endif
