

// Time in ms you need to hold down the button to be considered a long press
unsigned int longPressTime = 1000;

// How many times you need to hit the button to be considered a multi-hit
byte multiHitTarget = 2;

// How fast you need to hit all buttons to be considered a multi-hit
unsigned int multiHitTime = 400;

//// Create an instance of DailyStruggleButton
//DailyStruggleButton myButton;
//
//void setup(){
////  Serial.begin(9600);
//  Serial.begin(115200);
//
//  // Use set(digital pin connected to button, a callback function, type of pull-up/down) to initialise the button
//  // Choose between INT_PULL_UP, EXT_PULL_UP and EXT_PULL_DOWN
////  myButton.set(4, buttonEvent, INT_PULL_UP);
////  myButton.set(NOT_RELEASE_MOTOR_BUTTON, buttonEvent, INT_PULL_UP);
////
////
////
////  // You can enable long press to use this feature
////  myButton.enableLongPress(longPressTime);
////
////  // You can enable multi-hit to use this feature
////  myButton.enableMultiHit(multiHitTime, multiHitTarget);
//}


// This function will be called whenever an event occurs.
// We pass the name of this callback function in set().
// It needs to take a parameter of the byte datatype.
// This byte will indicate the event.
// It needs to return void.
void buttonEvent(byte btnStatus) {

  // We can use switch/case to run through what happens for each event
  switch (btnStatus) {

    // onPress is indicated when the button is pushed down
    case onPress:
      Serial.println("Button Pressed");
      disableRA_Stepper();    //So that Right Assention motor can be manual set.
      Serial.println("RELEASE_MOTOR ");
      break;

    // onRelease is indicated when the button is let go
    case onRelease:
      Serial.println("Button Released");
//      disableRA_Stepper();    //So that Right Assention motor resumes hold/track.
      rightAssentionStepper.takeMicroStep(isNorthTracking); //Take a step to energize motor
      Serial.println("ENGAGE_MOTOR ");
      break;

    // onHold is indicated whenever the button is held down.
    // It can be annoying so we will comment this out in this example.
    //  case onHold:
    //    Serial.println("Button Long Pressed");
    //    break;

    // onLongPress is indidcated when you hold onto the button
    // more than longPressTime in milliseconds
    case onLongPress:
      Serial.print("Buttong Long Pressed For ");
      Serial.print(longPressTime);
      Serial.println("ms");
      disableRA_Stepper();    //So that Right Assention motor can be manual set.
      Serial.println("RELEASE_MOTOR ");
      break;

    // onMultiHit is indicated when you hit the button
    // multiHitTarget times within multihitTime in milliseconds
    case onMultiHit:
      Serial.print("Button Pressed ");
      Serial.print(multiHitTarget);
      Serial.print(" times in ");
      Serial.print(multiHitTime);
      Serial.println("ms");
      Serial.println("Tracking true ");
      isTracking = true; //Then turn off tracking.
      rightAssentionStepper.takeMicroStep(isNorthTracking); //Take a step to energize motor

      break;
  }
}
