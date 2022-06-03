/*
   Moddled on Serial Event methods and ISR from the https://docs.arduino.cc/built-in-examples/communication/SerialEvent

*/


String inputString = "Initilized string.";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
//extern bool stringComplete;

/* initialize serial:  */
void setupSerialInput() {
  inputString.reserve(200);         //  reserve 200 bytes for the inputString:
}//end setupSerialInput


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
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
      stringComplete = true;
    }
  }
}// end serialEvent


/*
    Get input string terminated by LF
*/

//bool checkCommandInput() {
//  // print the string when a newline arrives:
//  if (stringComplete) {
//    Serial.println("I got a command");
//    Serial.print("Here is the command: ");
//    Serial.println(inputString);
//    Serial.println("Hurray!");
//    Serial.println(inputString);
//    // clear the string:
//    inputString = "";
//    stringComplete = false;
//    return (stringComplete);
//  }
//}
