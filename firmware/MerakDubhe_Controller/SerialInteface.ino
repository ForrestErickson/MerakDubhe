/*
   Moddled on Serial Event methods and ISR from the https://docs.arduino.cc/built-in-examples/communication/SerialEvent
  //Library made by Forrest Erickson
*/

String inputString = "";         // a String to hold incoming data
bool stringIsComplete = false;  // whether the string is complete
//extern bool stringComplete;

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
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringIsComplete = true;
    }
  }
}// end serialEvent

// print the string when complete with newline:
void checkCommandInput() {
  if (stringIsComplete) {
    Serial.print("Here is the command: ");
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringIsComplete = false;
  }
}
