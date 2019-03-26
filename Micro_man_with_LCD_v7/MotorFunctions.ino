//function for the oled
void drawtext(String axis_, float speed_axis_ , float dirState_ , bool isHome_){
  //tft.fillScreen(ST7735_BLACK);
   oled.clearBuffer();
   oled.setCursor(0,10);
   oled.print("Axis: ");
   oled.println(axis_);
   oled.setCursor(0,20);
   oled.print("Curr freq: ");
   oled.println(speed_axis_);
   oled.setCursor(0,30);
   oled.print("Direction: ");
   oled.println(dirState_);
   if (isHome_){
    oled.setCursor(0,40);
    oled.println("HOME");
   }
   
   oled.sendBuffer();
}

void setRotation(uint8_t rotation);

//function to run the motor. Takes number of of steps
//command in the format #steps + '*' (et. 100*, 100 steps)
void runComm(int steps_, int motor_){
  //Serial.println(st);
  int cycle_period = int((1000000/signal_freq)+0.5);
  //Serial.println(cycle_period);
  int run_time = (1.0/signal_freq)*steps_*1000000; //secs for steps in micros();
  //int run_time = stepsToTime(st);
  //Serial.println(run_time);

  //unsigned long startTime = micros();
  //unsigned long endTime = startTime + run_time;
  //digitalWrite(dir1,0);
  //for(int k = 0; k <= 1; k++){
    isHome = false;
    for(int u = 0; u <= (steps_); u++){
      unsigned long startTime = micros();
      digitalWrite(motor_,HIGH);
      while(micros()<(startTime+cycle_period/2));
      digitalWrite(motor_,LOW);
      while(micros()<startTime+cycle_period);
      hall_reading = analogRead(hall);
      Serial.println(hall_reading);
     // if (x_home(hall_reading)){  //if the hall sensor pass a certain threshold then this exit the for loop
      //  isHome = 1;
       // Serial.println("Here");
       // break;
      //}
    } 
    //digitalWrite(dir1,1);
  //}
}

//convert to microseconds time for number of steps using the set frequency
double stepsToTime(int number_of_steps, int PWM_freq){
  double period = (1.0/PWM_freq) * pow(10,9);
  return (period*number_of_steps);
}
