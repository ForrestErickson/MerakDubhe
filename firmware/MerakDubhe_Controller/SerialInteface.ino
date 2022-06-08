/*
   Moddled on Serial Event methods and ISR from the https://docs.arduino.cc/built-in-examples/communication/SerialEvent
  //Library made by Forrest Erickson
*/

String inputString = "";         // a String to hold incoming data
// bool stringIsComplete = false;  // whether the string is complete
//extern bool stringComplete;

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
  inputString.reserve(200);         //  reserve 200 bytes for the inputString:
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
    Serial.println(inputString);
    strcpy(tempChars, receivedChars);

    //Now parse commands to do something
    parseData(); //messageFromPC, Int from PC and Float from PC.
//    showParsedData();
    processCommands();
    // After proccessing command, clear the string and the flag
    inputString = "";
    newData = false;
  }//end newData
}// end checkCommandInput()

/* Split the data into its parts
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
  Serial.print("Message ");
  Serial.println(messageFromPC);
  Serial.print("Integer ");
  Serial.println(integerFromPC);
  Serial.print("Float ");
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
  char COMMAND[32];
  char str2[32];
  int ret;
  //Command Strings
  //strcpy(COMMAND, "WAVE");
  //  strcpy(str2, "ABCDEF");
  //  strcpy(str2, messageFromPC );
  //ret = strcmp(str1, str2);

  ret = strcmp("WAVE", messageFromPC);
  if (ret < 0) {
    Serial.println("Message is less than COMMAND");
  } else if (ret > 0) {
    Serial.println("Message is Greater than COMMAND");
  } else {
    Serial.println("Message is equal to COMMAND");
    if (integerFromPC != 0) {
      wave(integerFromPC); //wave with number of steps.
    } else {
      wave(1);
    }
  }// end else equality.

  //Print help menu
  ret = strcmp("HELP", messageFromPC);
  if (ret < 0) {
    Serial.println("Message is less than HELP");
  } else if (ret > 0) {
    Serial.println("Message is Greater than HELP");
  } else {
    Serial.println("Message is equal to HELP");
    commandMenu();
  }// end Print help menu

//Set or clear Break
//<BREAK, 1> or <BREAK, 0>
  ret = strcmp("BREAK", messageFromPC);
 if (ret < 0) {
    Serial.println("Message is less than BREAK");
  } else if (ret > 0) {
    Serial.println("Message is Greater than BREAK");
  } else {
    Serial.println("Message is equal to BREAK");
    if (integerFromPC != 0) {
      rightAssentionStepper.hold(); //Set Break
    } else {
      rightAssentionStepper.disable(); //Release Break
    }
  }// end set or clear break

//Report count
//<count> 
  ret = strcmp("count", messageFromPC);
 if (ret < 0) {
    Serial.println("Message is less than count");
  } else if (ret > 0) {
    Serial.println("Message is Greater than count");
  } else {
    Serial.println("Message is equal to 'count'");
 //
 Serial.print("Count = ");
 Serial.println(rightAssentionStepper.counter());
  }// end set or clear break

//Take single step
//<STEP, 1> or <STEP, 0> for forward or back
  ret = strcmp("STEP", messageFromPC);
 if (ret < 0) {
    Serial.println("Message is less than STEP");
  } else if (ret > 0) {
    Serial.println("Message is Greater than STEP");
  } else {
    Serial.println("Message is equal to STEP");
    if (integerFromPC != 0) {
      rightAssentionStepper.takestep(true); //Set Break
    } else {
      rightAssentionStepper.takestep(false); //Release Break
    }
  }// end set or clear break

}// end processCommands

void commandMenu()  {
  Serial.println("\f\n===== MerakDubhe Command Menu =====") ;
  Serial.println("HELP for this menu.");
  Serial.println("BREAK, 1/0 for electronic break on/off.");
  Serial.println("STEP, 1/0 for step forward or back.");
  Serial.println("count, report count.");
  Serial.println("F for Forward.");
  Serial.println("R for Reverse.");
  Serial.println("WAVE, 'n' for Wave motor forward and back by 'n'.");
  Serial.println("H for Home the trolly.");
  Serial.println("G for motor and photos Go.");
  Serial.println("s for motor and photos sTOP.");
  Serial.println("S to set motor Speed.");
  Serial.println("T to report Time to travel rail.");
  Serial.println("L/l increment or decrement percent Length of rail to travel");
  Serial.println("E to set Exposure in seconds.");
  Serial.println("A to trigger Auto Focus on camera.");
  Serial.println("P to make Photo now!");
  Serial.println("I to set photo Interval.");
  Serial.println("N to set Number of photos during rail travel.");
  Serial.println("M to refresh the Menu.");
  Serial.print("System stepsPerRevolution: ");
  Serial.println(stepsPerRevolution);
  Serial.println(""); //Leave space after menu.

}// end commandMenu

//Note 24*60*60= 86,400
//200 step per rev motor times 20 reves per RA turn is 4000
// 86,400 seconds / 4000 steps is 21.6 seconds per step
// if we devide by 256 we have 84.375 mSeconds / step.
