void serialEvent() {
  String temp;
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '*') {
      stringComplete = true;
      break;
    }
    else{
      commandString += inChar;
    }
  }
}

bool x_home() {
  hall_reading = analogRead(hall);
  if (hall_reading < 1000){
    return true;
  }
}

