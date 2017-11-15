#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

//Teensy definitions
#define speed_control 14 //map with the PWM frequency
#define stp 10 //PWM
#define ms1 9
#define ms2 8
#define ms3 7
#define slp 6 //inverting pin
#define res 5 //inverting pin, need to set high for the outputs to work
#define ena 4 //inverting pin
#define dir 3
#define but 2
#define dirBut 1

#define TFT_CS     15
#define TFT_RST    16  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to -1!
#define TFT_DC     17

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

int signal_freq = 500;  //set the initial speed
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
  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(1);
  
  pinMode(ms1,OUTPUT);
  pinMode(ms2,OUTPUT);  
  pinMode(ms3,OUTPUT);    
  pinMode(dir,OUTPUT);
  //pinMode(stp,OUTPUT);
  pinMode(slp,OUTPUT);
  pinMode(res,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(but,INPUT);
  pinMode(dirBut,INPUT);
  
  analogWriteFrequency(stp,signal_freq); 

  digitalWrite(ms1,HIGH);
  digitalWrite(ms2,HIGH);
  digitalWrite(ms3,HIGH);
  digitalWrite(dir,LOW);
  digitalWrite(slp,HIGH);
  digitalWrite(res,HIGH);
  
  Serial.begin(9600);
  commandString.reserve(200);
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
        signal_freq = map(potReading,0,1023,lowerBoundFreq,upperBoundFreq);
        oldReading = potReading;
        //Serial.println(signal_freq);
        //Serial.println("here");
        analogWriteFrequency(stp,signal_freq);
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
  //int run_time = (3200/signal_freq)*100*1000; //secs for 100 rotations
  //int start_run = millis(); 
  bool change = (change_speed||change_dir);
  drawtext(change, oldReading, potReading, lowerBoundFreq, upperBoundFreq, dirState);
  lastButtonState = buttonReading;
  lastDir = dirVar;
  digitalWrite(dir,dirState); 
  //analogWrite(stp,127);
  serialEvent();
  if (stringComplete){
    runComm();
    stringComplete = false;
    commandString = ""; 
  }
}

//Functions to write on the LCD
void drawtext(int buttonState, int oldReading1, int reading, int lowerBoundFreq, int upperBoundFreq, int dirState){
  if (buttonState){
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.print("Current freq:");
    tft.println(map(oldReading1,0,1023,lowerBoundFreq,upperBoundFreq));
    tft.setCursor(0,50);
    tft.print("Set freq:");
    tft.println(map(reading,0,1023,lowerBoundFreq,upperBoundFreq));
    tft.print("Direction: ");
    tft.println(dirState);
    //setReading = reading;      
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
  int cycle_period = int((100000/signal_freq)+0.5);
  int run_time = (1/signal_freq)*st*1000000; //secs for steps in micros();

  unsigned long startTime = micros();
  unsigned long endTime = startTime + run_time;
  while (micros() < endTime){
    analogWrite(stp,127);
  }  
}

//convert to microseconds time for number of steps using the set frequency
double stepsToTime(int number_of_steps, int PWM_freq){
  double period = (1/PWM_freq) * pow(10,9);
  return (period*number_of_steps);
}


