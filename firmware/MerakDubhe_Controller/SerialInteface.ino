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
  //  }// end old serial avaialbe()

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

  //if (stringIsComplete) {
  if (newData) {
    Serial.print("Here is the command: ");
    Serial.println(inputString);

    strcpy(tempChars, receivedChars);
    //    strcpy(tempChars, inputString);

    //Now parse commands to do something
    parseData(); //messageFromPC, Int from PC and Float from PC.
    showParsedData();

    processCommands();

    // clear the string:
    inputString = "";
    newData = false;
    //stringIsComplete = false;


  }
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


//Command Strings
//strcpy(str1, "abcdef");
//strcpy(str2, "ABCDEF");

/*messageFromPC, Int from PC and Float from PC. */
void processCommands() {
  char str1[32];
  char str2[32];
  int ret;
  //Command Strings
  strcpy(str1, "WAVE");
  //  strcpy(str2, "ABCDEF");
  strcpy(str2, messageFromPC );
  //ret = strcmp(str1, str2);
  ret = strcmp(str1, messageFromPC);

  if (ret < 0) {
    Serial.println("str1 is less than str2");
  } else if (ret > 0) {
    Serial.println("str2 is less than str1");
  } else {
    Serial.println("str1 is equal to str2");
    if (integerFromPC != 0) {
      wave(integerFromPC);
    } else {
      wave(1);
    }

  }




}// end processCommands
