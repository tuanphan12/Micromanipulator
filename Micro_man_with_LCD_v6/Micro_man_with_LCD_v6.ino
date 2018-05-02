#include <SPI.h>
#include <U8g2lib.h>
#define DELAY 300

#define speed_control 34 //map with the PWM frequency
#define stp1 32 //motor 1 (platform)
#define stp2 22  //motor 2 (bottom axis)
#define stp3 21  //motor 3 (z-axis)
#define ms1 33
#define ms2 25
#define ms3 26
#define slp 27 //inverting pin
#define res 14 //inverting pin, need to set high for the outputs to work
#define ena 12 //inverting pin
#define dir 13 //direction
#define button_1 4
#define button_2 2 

#define SHORT_PRESS 1
#define LONG_PRESS 2

// Set up the oled object
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled(U8G2_R0, 5, 17, 16);

int channel = 0;
int resolution = 8;

int signal_freq = 2000;  //set the initial speed
int buttonReading_1;
int buttonReading_2;
signed long execute_time;
int upperBoundFreq = 1000;
int lowerBoundFreq = 100;
int potReading = 0;
int oldReading = signal_freq; 
int thres = 25;
int setReading;
int dirState = 0;  //1 is pushing out; 0 is pulling in
bool stringComplete = false;
int axis;
int motorState = 0;
String axisStr = "";
int st;  //number of steps

String commandString;

class Button{
  public:
  unsigned long t_of_state_2;
  unsigned long t_of_button_change;    
  unsigned long debounce_time;
  unsigned long long_press_time;
  int pin;
  int flag;
  bool button_pressed;
  int state; // This is public for the sake of convenience
  Button(int p) {
    flag = 0;  
    state = 0;
    pin = p;
    t_of_state_2 = millis(); //init
    t_of_button_change = millis(); //init
    debounce_time = 10;
    long_press_time = 1000;
    button_pressed = 0;
  }
  void read() {
    int button_state = digitalRead(pin);  
    button_pressed = !button_state;
  }
  
  int update() {
    read();
    flag = 0;
    if (state==0) { // Unpressed, rest state
      if (button_pressed) {
        state = 1;
        t_of_button_change = millis();
      }
    } else if (state==1) { //Tentative pressed
      if (!button_pressed) {
        state = 0;
        t_of_button_change = millis();
      } else if (millis()-t_of_button_change >= debounce_time) {
        state = 2;
        t_of_state_2 = millis();
      }
    } else if (state==2) { // Short press
      if (!button_pressed) {
        state = 4;
        t_of_button_change = millis();
      } else if (millis()-t_of_state_2 >= long_press_time) {
        state = 3;
      }
    } else if (state==3) { //Long press
      if (!button_pressed) {
        state = 4;
        t_of_button_change = millis();
      }
    } else if (state==4) { //Tentative unpressed
      if (button_pressed && millis()-t_of_state_2 < long_press_time) {
        state = 2; // Unpress was temporary, return to short press
        t_of_button_change = millis();
      } else if (button_pressed && millis()-t_of_state_2 >= long_press_time) {
        state = 3; // Unpress was temporary, return to long press
        t_of_button_change = millis();
      } else if (millis()-t_of_button_change >= debounce_time) { // A full button push is complete
        state = 0;
        if (millis()-t_of_state_2 < long_press_time) { // It is a short press
          flag = 1;
        } else {  // It is a long press
          flag = 2;
        }
      }
    }
    return flag;
  }
};

Button firstButton(button_1);
Button secondButton(button_2);

void setup() {
  // put your setup code here, to run once:

  oled.begin();
  oled.setFont(u8g2_font_5x7_tf);    
  
  Serial.begin(115200);
  commandString.reserve(200);
  pinMode(ms1,OUTPUT);
  pinMode(ms2,OUTPUT);  
  pinMode(ms3,OUTPUT);    
  pinMode(dir,OUTPUT);
  pinMode(slp,OUTPUT);
  pinMode(res,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(stp1,OUTPUT);
  pinMode(stp2,OUTPUT);
  pinMode(stp3,OUTPUT);
  pinMode(button_1,INPUT_PULLUP);
  pinMode(button_2,INPUT_PULLUP);

  digitalWrite(ms1,HIGH);
  digitalWrite(ms2,HIGH);
  digitalWrite(ms3,HIGH);
  digitalWrite(dir,LOW);
  digitalWrite(slp,HIGH);
  digitalWrite(res,HIGH);
}

void loop() {
  int change_speed = 0;
  int change_dir = 0;
  buttonReading_1 = firstButton.update();
  buttonReading_2 = secondButton.update();
  potReading = analogRead(speed_control); 

  //for switching platform
  switch (motorState){
    case 0: //axis 1 (mounting platform)
      axis = stp1;
      axisStr = "Mounting Platform";      
      if (buttonReading_1 == SHORT_PRESS){
        motorState = 1;
      }
      break;
    case 1: //axis 2 (bottom platform)
      axis = stp2;
      axisStr = "Bottom Platform";        
      if (buttonReading_1 == SHORT_PRESS){
        motorState = 2;  
      }
      break;   
    case 2: //axis 2 (bottom platform)
      axis = stp3;
      axisStr = "Z-Axis";        
      if (buttonReading_1 == SHORT_PRESS){
        motorState = 0;  
      }
      break;    
  }

  //change frequency of the motor
  if (buttonReading_2 == SHORT_PRESS){
    oldReading = potReading; //updated frequency
    signal_freq = map(potReading,0,4095,lowerBoundFreq,upperBoundFreq);    
  }

  //change direction. Same pin
  if (buttonReading_2 == LONG_PRESS){
    dirState = (dirState + 1) % 2;
    change_dir = 1;
  } 
  
  //for display not to refresh all the time
  if ((potReading > setReading + thres) || (potReading < setReading - thres)){
    setReading = potReading;
    change_speed = 1;
  }
  
  bool change = (change_speed||change_dir);
  drawtext(change, axisStr, oldReading, potReading, lowerBoundFreq, upperBoundFreq, dirState);

  digitalWrite(dir,dirState); 
  //analogWrite(stp,127);
  //delay(500);
  serialEvent();
  if (stringComplete){
    Serial.print("frequency:");
    Serial.println(signal_freq);
    //Serial.println(potReading);
    //Serial.println(commandString);
    st = commandString.toInt();
    runComm(st,axis);
    stringComplete = false;
    commandString = ""; 
  }
  
  if (buttonReading_1 == LONG_PRESS){
    runComm(st,axis);
  }
  
}
