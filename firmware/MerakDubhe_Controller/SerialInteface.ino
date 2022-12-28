/*
   Moddled on Serial Event methods and ISR from the https://docs.arduino.cc/built-in-examples/communication/SerialEvent
  //Library made by Forrest Erickson
*/

//Notes on useing PROGMEM at: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
#include <avr/pgmspace.h>

//Menu Stings Into PROGMEM
//                             1234567890123456789012345678901234567890    //40 characters max
const char MENU_0[] PROGMEM = "======= MerakDubhe Command Menu ========"; // "String 0" etc are strings to store - change to suit.
const char MENU_1[] PROGMEM = "        ===== 20221227 22:31 =====";
const char MENU_2[] PROGMEM = "HELP for this menu.";
const char MENU_3[] PROGMEM = "BREAK, 1/0 for electronic break on/off";
const char MENU_4[] PROGMEM = "STEP, 1/0 for step forward or back";
const char MENU_5[] PROGMEM = "COUNT, report count";
const char MENU_6[] PROGMEM = "WAVE, 'n',Wave motor to&fro by 'n'";
const char MENU_7[] PROGMEM = "MICROWAVE, 'n' microWave to&fro by 'n'";
const char MENU_8[] PROGMEM = "MICRO, 'n' Single 256 micro step";
const char MENU_9[] PROGMEM = "REV, 1/0 Revolve 200 steps";
const char MENU_10[] PROGMEM = "MICROREV, 1/0 Rev microSteps=200*256";
const char MENU_11[] PROGMEM = "STOP, stops stepping and disables motor";
const char MENU_12[] PROGMEM = "TRACK, 1/0 for tracking on / off";
const char MENU_13[] PROGMEM = "NORTH, 1/0 for northern or southern";
const char MENU_14[] PROGMEM = "FOCUS";
const char MENU_15[] PROGMEM = "EXPOSURE, 'n' in seconds.";
const char MENU_16[] PROGMEM = "TIMELAPS, 1/0 to make time laps";


// Then set up a table to refer to your MENU strings.
const char *const MENU_table[] PROGMEM = {MENU_0, MENU_1, MENU_2, MENU_3, MENU_4, MENU_5, MENU_6, MENU_7, MENU_8, MENU_9, MENU_10, MENU_11, MENU_12, MENU_13, MENU_14, MENU_15, MENU_16};
char buffer[40];  // make sure this is large enough for the largest string it must hold


extern float microStepPeriod;

extern bool isTracking ; //
extern bool isNorthTracking;
// String inputString = "";         // a String to hold incoming data

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;

void commandMenu()  {
  Serial.println(""); //Insert space before menu.
  for (int i = 0; i < (sizeof(MENU_table) / sizeof(MENU_table[0])); i++) {
    strcpy_P(buffer, (char *)pgm_read_word(&(MENU_table[i])));  // Necessary casts and dereferencing, just copy.
    Serial.println(buffer);
  }
  myCanonT3.printExposureTime();  //Print exposure time
  Serial.println(""); //Leave space after menu.
}// end commandMenu


/* initialize serial:  */
void setupSerialInput() {
  //  inputString.reserve(200);         //  reserve 200 bytes for the inputString:
}//end setupSerialInput


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.

  Get input string terminated by LF and signals with StringComplete.
*/
void serialEvent() {
  //  while (Serial.available()) {
  //    // get the new byte:
  //    char inChar = (char)Serial.read();
  //    // add it to the inputString:
  //    inputString += inChar;
  //    // if the incoming character is a newline, set a flag so the main loop can
  //    // do something about it:
  //    if (inChar == '\n') {
  //      stringIsComplete = true;   // aka newData = true;
  //    }
  //  }// end old serial avaialbe() terminted with LF only.

  char rc;
  while (Serial.available() > 0 && newData == false) {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {              //Received endMarker so terminate with NULL. Update state variabls.
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }//end of RecvInProgress
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }//end serial avaiable and new data false.
}// end serialEvent

// print the string when complete with newline:
//Make the command so something
void checkCommandInput() {
  if (newData) {
    //    Serial.print("Here is the command: ");
    //    Serial.println(receivedChars);
    strcpy(tempChars, receivedChars);

    //Now parse commands to do something
    parseData(); //messageFromPC, Int from PC and Float from PC.
    //    showParsedData();
    processCommands();
    // After proccessing command, clear the string and the flag
    //    inputString = "";
    newData = false;
  }//end newData
}// end checkCommandInput()

/* Split the ',' delimited data into its parts
  Reference on using "strok" see: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
  char *strcpy(char *dest, const char *src)
  Copies the string pointed to, by src to dest.
*/
void parseData() {      // split the data into its parts
  char * strtokIndx; // this pointer is set by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  integerFromPC = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ",");
  floatFromPC = atof(strtokIndx);     // convert this part to a float

}//end parseData()

//============

//For parsing debug
//void showParsedData() {
//  Serial.print("Message= ");
//  Serial.print(messageFromPC);
//  Serial.print(", Integer= ");
//  Serial.print(integerFromPC);
//  Serial.print(", Float= ");
//  Serial.println(floatFromPC);
//}//end showParsedData()

//============


/* Global string messageFromPC.  */
/*Serial in comand in the formate: */
/*chararray messageFromPC, Int from PC and Float from PC. */
// example srings
// <WAVE, 5, 20>
//<HELP>

void processCommands() {
  //Command Strings. Use strcmp which returns zero on a match.
  // WAVE command
  if (!strcmp("WAVE", messageFromPC)) {
    //    Serial.println("Message is equal to WAVE");
    if (integerFromPC != 0) {
      wave(integerFromPC); //wave with number of steps.
    } else {
      wave(1);
    }
  }// end else equality.

  // MICROWAVE command
  if (!strcmp("MICROWAVE", messageFromPC)) {
    //    Serial.println("Message is equal to MICROWAVE");
    if (integerFromPC != 0) {
      microWave(integerFromPC); //wave with number of steps.
    } else {
      microWave(1);
    }
  }// end else equality.

  //Print help menu
  if (!strcmp("HELP", messageFromPC)) {
    //    Serial.println("Message is equal to HELP");
    commandMenu();
  }//end help menu

  //Set or clear Break
  //<BREAK, 1> or <BREAK, 0>
  if (!strcmp("BREAK", messageFromPC)) {
    //   Serial.println("Message is equal to BREAK");
    if (integerFromPC != 0) {
      rightAssentionStepper.hold(); //Set Break
    } else {
      rightAssentionStepper.disable(); //Release Break
    }
  }// end set or clear break

  //Report COUNT
  //<COUNT>
  if (!strcmp("COUNT", messageFromPC)) {
    //    Serial.println("Message is equal to 'count'");
    Serial.print("Count = ");
    Serial.println(rightAssentionStepper.counter());
  }// end report count.

  //Take single step
  //<STEP, 1> or <STEP, 0> for forward or back
  if (!strcmp("STEP", messageFromPC)) {
    //    Serial.println("Message is equal to STEP");
    if (integerFromPC != 0) {
      rightAssentionStepper.takestep(true); //step forward
    } else {
      rightAssentionStepper.takestep(false); //step back
    }
  }// end single step

  //Revolution of motor, 200 steps
  //<REV, 1> or <REV, 0> for forward or back
  if (!strcmp("REV", messageFromPC)) {
    //   Serial.println("Message is equal to REV");
    if (integerFromPC != 0) {//800 quarter steps forward
      for (int i = 0; i < 800; i++) {
        rightAssentionStepper.takestep(true); //step forward
      }
    } else {
      for (int i = 0; i < 800; i++) {
        rightAssentionStepper.takestep(false); //step back
      }
    }//end else
    rightAssentionStepper.disable();
  }// end REV

  //Micro Revolution of motor, 200*256 micro steps
  //<MICROREV, 1> or <MICROREV, 0> for forward or back
  if (!strcmp("MICROREV", messageFromPC)) {
    long REV_SIZE = 200 * 256L;
    //    Serial.println("Message is equal to MICROREV");
    //    Serial.print("REV_SIZE is: ");
    //    Serial.println(REV_SIZE);

    if (integerFromPC != 0) {//800 quarter steps forward
      for (long i = 0; i < (REV_SIZE); i++) {
        rightAssentionStepper.takeMicroStep(true); //step forward
      }// end steps
    } else {
      for (long i = 0; i < (REV_SIZE); i++) {
        rightAssentionStepper.takeMicroStep(false); //step back
      }// end steps
    }//end else
    rightAssentionStepper.disable();
  }// end MICROREV

  //Take micro step 255
  //<STEP, 1> or <STEP, 0> for forward or back
  if (!strcmp("MICRO", messageFromPC)) {
    //    Serial.println("Message is equal to MICRO");
    rightAssentionStepper.takeMicroStep(integerFromPC);
  }// end single step

  //STOP tracking state. Disable electronic break.
  //<STOP> for stopping tracking of RA
  if (!strcmp("STOP", messageFromPC) || !strcmp("stop", messageFromPC)) {
    //   Serial.println("Message is equal to STOP");
    isTracking = false;
    TrackingLED(false);
    rightAssentionStepper.disable();
  }// end set/clear Tracking

  //Set TRACK state variable
  //<TRACK, 1> or <TRACK, 0> for starting or stopping tracking of RA
  if (!strcmp("TRACK", messageFromPC)) {
    //   Serial.println("Message is equal to TRACK");
    if (integerFromPC != 0) {
      isTracking = true;
      TrackingLED(true);
    } else {
      isTracking = false;
      TrackingLED(false);
    }
  }// end set/clear Tracking  TIMELAPS

  //Set NORTH state variable
  //<NORTH, 1> or <NORTH, 0> for direction of tracking of RA
  if (!strcmp("NORTH", messageFromPC)) {
    //   Serial.println("Message is equal to NORTH");
    if (integerFromPC != 0) {
      isNorthTracking = true;
    } else {
      isNorthTracking = false;
    }
  }// end set/clear North direction of tracking

  //Make photograph
  if (!strcmp("PHOTO", messageFromPC)) {
    myCanonT3.makePhoto();
    //    myCanonT3.focusAndPhoto();
  }// end focus

  //Set focus
  if (!strcmp("FOCUS", messageFromPC)) {
    myCanonT3.setAutoFocus();
  }// end focus

  //Set exposure in seconds
  if (!strcmp("EXPOSURE", messageFromPC)) {
       Serial.print("Message equal to EXPOSURE:");
    if (integerFromPC <= 0) {
      //Set minimum;
      myCanonT3.setExposureTimeSeconds(1);
    } else {
      myCanonT3.setExposureTimeSeconds(integerFromPC);
       Serial.print("Value FROM pc is equal to EXPOSURE:");
       Serial.println(integerFromPC);
    }
  }// end set/clear North direction of tracking

  //Set TIMELAPS state variable
  //<TIMELAPS, 1> or <TIMELAPS, 0> for starting or stopping time laps photography
  if (!strcmp("TIMELAPS", messageFromPC)) {
    //   Serial.println("Message is equal to TRACK");
    if (integerFromPC != 0) {
      isTimeLaps = true;  //Set making photos.
    } else {
      myCanonT3.setLastExposure(true); //Set flag so that after expsoure and write to camera end time laps.
    }
  }// end set/clear Tracking  TIMELAPS

}// end processCommands


//Note 24*60*60= 86,400
//200 step per rev motor times 20 reves per RA turn is 4000
// 86,400 seconds / 4000 steps is 21.6 seconds per step
// if we devide by 256 we have 84.375 mSeconds / step.
// 4000 * 256 = 1,024,000 steps around the sky.
