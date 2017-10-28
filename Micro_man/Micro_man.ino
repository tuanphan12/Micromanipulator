//Teensy definitions
#define speed_control 14 //map with the PWM frequency
#define ena 12 //inverting pin
#define dir 11
#define stp 10 //PWM
#define ms1 9
#define ms2 8
#define ms3 7
#define slp 6 //inverting pin
#define res 5 //inverting pin, need to set high for the outputs to work
#define but 4


int signal_freq = 2000;
int cycle_period = int((100000/signal_freq)+0.5);
int buttonReading = 1;
signed long execute_time;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 15;
int buttonState = 0;
int lastButtonState = 0;
int steps = 5;  //how many steps to run
int boundState = 1;
int upperBoundFreq = 20000;
int lowerBoundFreq = 2000;
int potReading = 0;

void setup() {
  pinMode(ms1,OUTPUT);
  pinMode(ms2,OUTPUT);  
  pinMode(ms3,OUTPUT);    
  pinMode(dir,OUTPUT);
  //pinMode(stp,OUTPUT);
  pinMode(slp,OUTPUT);
  pinMode(res,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(but,INPUT);
  
  analogWriteFrequency(stp,signal_freq); 

  digitalWrite(ms1,HIGH);
  digitalWrite(ms2,HIGH);
  digitalWrite(ms3,HIGH);
  digitalWrite(dir,HIGH);
  digitalWrite(slp,HIGH);
  digitalWrite(res,HIGH);
  
  Serial.begin(9600);
}

void loop() {
  buttonReading = digitalRead(but);
  potReading = analogRead(speed_control);
  if (buttonReading != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - debounceDelay) > lastDebounceTime){
    if (buttonReading != buttonState){
      buttonState = buttonReading;
      if (buttonState == HIGH){
        Serial.print("Pot reading:");
        Serial.println(potReading);
        signal_freq = map(potReading,0,1023,lowerBoundFreq,upperBoundFreq);
        Serial.println(signal_freq);
        Serial.println("here");
        analogWriteFrequency(stp,signal_freq);
      } 
    }
  }
  lastButtonState = buttonReading;
  analogWrite(stp,127);
}

//Functions to write on the LCD
//void drawtext(char text, uint16_t color){
//  tft.setCursor(0, 0);
//  tft.setTextColor(color);
//  tft.setTextWrap(true);
//  tft.print(text);
//}

