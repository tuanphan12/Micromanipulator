#include <SPI.h>
#include <U8g2lib.h>
#define DELAY 300

#define speed_control 34 //map with the PWM frequency
#define stp 32 //PWM
#define ms1 33
#define ms2 25
#define ms3 26
#define slp 27 //inverting pin
#define res 14 //inverting pin, need to set high for the outputs to work
#define ena 12 //inverting pin
#define dir 13

#define but 4
#define dirBut 2 

// Set up the oled object
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled(U8G2_R0, 5, 17, 16);

int channel = 0;
int resolution = 8;

int signal_freq = 1000;  //set the initial speed
int buttonReading = 1;
signed long execute_time;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 10;
int buttonState = 0;
int lastButtonState = 0;
int boundState = 1;
int upperBoundFreq = 1000;
int lowerBoundFreq = 100;
int potReading = 0;
int oldReading; 
int change;
int thres = 10;
int setReading;
int dirVar;
int lastDir = 0;
int dirState = 0;
int iniDir = 0;
bool stringComplete = false;

String commandString;

void setup() {
  // put your setup code here, to run once:

  oled.begin();
  oled.setFont(u8g2_font_ncenB10_tr);    
  
  Serial.begin(115200);
  commandString.reserve(200);
  pinMode(ms1,OUTPUT);
  pinMode(ms2,OUTPUT);  
  pinMode(ms3,OUTPUT);    
  pinMode(dir,OUTPUT);
  pinMode(slp,OUTPUT);
  pinMode(res,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(stp,OUTPUT);
  pinMode(but,INPUT);
  pinMode(dirBut,INPUT);

  digitalWrite(ms1,HIGH);
  digitalWrite(ms2,HIGH);
  digitalWrite(ms3,HIGH);
  digitalWrite(dir,LOW);
  digitalWrite(slp,HIGH);
  digitalWrite(res,HIGH);

  //ledcSetup(channel, signal_freq, resolution);
  //ledcAttachPin(stp,channel);
  //ledcWrite(channel,255);
}

void loop() {
  int change_speed = 0;
  int change_dir = 0;
  buttonReading = digitalRead(but);
  dirVar = digitalRead(dirBut);
  potReading = analogRead(speed_control); 

  bool dirCon = (dirVar != lastDir);
    
  if ((buttonReading != lastButtonState) || dirCon){
    lastDebounceTime = millis();
  }
  if ((millis() - debounceDelay) > lastDebounceTime){
    if (buttonReading != buttonState){
      buttonState = buttonReading;
      if (buttonState == HIGH){
        change = 1;
        //Serial.print("Pot reading:");
        //Serial.println(potReading);
        signal_freq = map(potReading,0,4095,lowerBoundFreq,upperBoundFreq);
        oldReading = potReading;
        Serial.println(signal_freq);
        //Serial.println("here");
        //ledcWriteTone(channel,signal_freq);
      } 
    }

    //changing direction
    if (dirVar != 0){
      if (dirState == LOW){
        dirState = HIGH;
        change_dir = 1;
      }
      else{
        dirState = LOW;
        change_dir = 1;
      }      
   }
  }

  //for display not to refresh all the time
  if ((potReading > setReading + thres) || (potReading < setReading - thres)){
    setReading = potReading;
    change_speed = 1;
  }
  bool change = (change_speed||change_dir);
  drawtext(change, oldReading, potReading, lowerBoundFreq, upperBoundFreq, dirState);
  lastButtonState = buttonReading;
  lastDir = dirVar;
  digitalWrite(dir,dirState); 
  //analogWrite(stp,127);
  //delay(500);
  serialEvent();
  if (stringComplete){
    Serial.print("frequency:");
    Serial.println(signal_freq);
    Serial.println(potReading);
    //Serial.println(commandString);
    runComm();
    stringComplete = false;
    commandString = ""; 
  }
}

//Functions to write on the LCD
void drawtext(int buttonState, int oldReading1, int reading, int lowerBoundFreq, int upperBoundFreq, int dirState){
  if (buttonState){
    //tft.fillScreen(ST7735_BLACK);
    oled.clearBuffer();
    oled.setCursor(0,15);
    //oled.setTextSize(2);
    oled.print("Curr freq: ");
    oled.println(map(oldReading1,0,4095,lowerBoundFreq,upperBoundFreq));
    oled.setCursor(0,35);
    oled.print("Set freq: ");
    oled.println(map(reading,0,4095,lowerBoundFreq,upperBoundFreq));
    oled.setCursor(0,55);
    oled.print("Direction: ");
    oled.println(dirState);
    //setReading = reading;  

    oled.sendBuffer();
    delay(300);
  }
}

void setRotation(uint8_t rotation);

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

//function to run the motor. Takes number of of steps
//command in the format #steps + '*' (et. 100*, 100 steps)
void runComm(){
  int st = commandString.toInt();
  //Serial.println(st);
  int cycle_period = int((1000000/signal_freq)+0.5);
  //Serial.println(cycle_period);
  int run_time = (1.0/signal_freq)*st*1000000; //secs for steps in micros();
  //int run_time = stepsToTime(st);
  //Serial.println(run_time);

  //unsigned long startTime = micros();
  //unsigned long endTime = startTime + run_time;
  for (int t = 0; t <= 1; t++){
    for(int u = 0; u <= (st); u++){
      unsigned long startTime = micros();
      digitalWrite(stp,HIGH);
      while(micros()<(startTime+cycle_period/2));
      digitalWrite(stp,LOW);
      while(micros()<startTime+cycle_period);
    } 
    digitalWrite(dir,HIGH);
  }
}

//convert to microseconds time for number of steps using the set frequency
double stepsToTime(int number_of_steps, int PWM_freq){
  double period = (1.0/PWM_freq) * pow(10,9);
  return (period*number_of_steps);
}
