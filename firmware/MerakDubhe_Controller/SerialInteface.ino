/*
   Moddled on Serial Event methods and ISR from the https://docs.arduino.cc/built-in-examples/communication/SerialEvent
  //Library made by Forrest Erickson
*/

extern bool isTracking ; //
// String inputString = "";         // a String to hold incoming data

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;

boolean newData = false;

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
    Serial.print("Here is the command: ");
    Serial.println(receivedChars);
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

void showParsedData() {
  Serial.print("Message= ");
  Serial.print(messageFromPC);
  Serial.print(", Integer= ");
  Serial.print(integerFromPC);
  Serial.print(", Float= ");
  Serial.println(floatFromPC);
}//end showParsedData()

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
    Serial.println("Message is equal to WAVE");
    if (integerFromPC != 0) {
      wave(integerFromPC); //wave with number of steps.
    } else {
      wave(1);
    }
  }// end else equality.

  //Print help menu
  if (!strcmp("HELP", messageFromPC)) {
    Serial.println("Message is equal to HELP");
    commandMenu();
  }//end help menu

  //Set or clear Break
  //<BREAK, 1> or <BREAK, 0>
  if (!strcmp("BREAK", messageFromPC)) {
    Serial.println("Message is equal to BREAK");
    if (integerFromPC != 0) {
      rightAssentionStepper.hold(); //Set Break
    } else {
      rightAssentionStepper.disable(); //Release Break
    }
  }// end set or clear break

  //Report COUNT
  //<COUNT>
  if (!strcmp("COUNT", messageFromPC)) {
    Serial.println("Message is equal to 'count'");
    Serial.print("Count = ");
    Serial.println(rightAssentionStepper.counter());
  }// end report count.

  //Take single step
  //<STEP, 1> or <STEP, 0> for forward or back
  if (!strcmp("STEP", messageFromPC)) {
    Serial.println("Message is equal to STEP");
    if (integerFromPC != 0) {
      rightAssentionStepper.takestep(true); //step forward
    } else {
      rightAssentionStepper.takestep(false); //step back
    }
  }// end single step

   //Take micro step 255
  //<STEP, 1> or <STEP, 0> for forward or back
  if (!strcmp("MICRO", messageFromPC)) {
    int *b;
    Serial.println("Message is equal to MICRO");
    b = rightAssentionStepper.steps1024(integerFromPC);
//    if (integerFromPC != 0) {
//      //steps1024(int count)
//      b = rightAssentionStepper.steps1024(0);
//      
//    } else {
//      b = rightAssentionStepper.steps1024(1);      
//    }

    Serial.println();
  }// end single step
  
  //Set TRACK state variable
  //<TRACK, 1> or <TRACK, 0> for starting or stopping tracking of RA
  if (!strcmp("TRACK", messageFromPC)) {
    Serial.println("Message is equal to TRACK");
    if (integerFromPC != 0) {
      //Set isTracking;
      isTracking = true;
    } else {
      //Clear isTracking;
      isTracking = false;
    }
  }// end set/clear Tracking

}// end processCommands

void commandMenu()  {
  Serial.println("\f\n===== MerakDubhe Command Menu =====") ;
  Serial.println("HELP for this menu.");
  Serial.println("BREAK, 1/0 for electronic break on/off.");
  Serial.println("STEP, 1/0 for step forward or back.");
  Serial.println("COUNT, report count.");
  Serial.println("WAVE, 'n' for Wave motor forward and back by 'n'.");
  Serial.println("TRACK, 1/0 for forward or back."); //Sets/clears isTracking
  Serial.println("SLEW, SPEED, DISTANCE"); // Speed is positive or negative. Distance in ?steps?
  Serial.println("GUIDE, SPEED, DISTANCE"); // Speed is positive or negative. Distance in ?steps?


//  Serial.println("F for Forward.");
//  Serial.println("R for Reverse.");
//  Serial.println("H for Home the trolly.");
//  Serial.println("G for motor and photos Go.");
//  Serial.println("s for motor and photos sTOP.");
//  Serial.println("S to set motor Speed.");
//  Serial.println("T to report Time to travel rail.");
//  Serial.println("L/l increment or decrement percent Length of rail to travel");
//  Serial.println("E to set Exposure in seconds.");
//  Serial.println("A to trigger Auto Focus on camera.");
//  Serial.println("P to make Photo now!");
//  Serial.println("I to set photo Interval.");
//  Serial.println("N to set Number of photos during rail travel.");
//  Serial.println("M to refresh the Menu.");
  
  Serial.print("System stepsPerRevolution: ");
  Serial.println(stepsPerRevolution);
  Serial.println(""); //Leave space after menu.

}// end commandMenu

//Note 24*60*60= 86,400
//200 step per rev motor times 20 reves per RA turn is 4000
// 86,400 seconds / 4000 steps is 21.6 seconds per step
// if we devide by 256 we have 84.375 mSeconds / step.
// 4000 * 256 = 1,024,000 steps around the sky.
