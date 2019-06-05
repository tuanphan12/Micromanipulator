#include <SPI.h>
#include <U8g2lib.h>
#include <Six302.h>
#include <math.h>
#include <esp32-hal.h>
#define DELAY 300

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

#define hall 35

#define SHORT_PRESS 1
#define LONG_PRESS 2

CommManager cm(50000,20000);

// Set up the oled object
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled(U8G2_R0, 5, 17, 16);

int channel = 0;
int resolution = 8;

signed long execute_time;
int axis;
int motorState = 0;
String axisStr = "";

int hall_reading = 0;
String commandString;

float dx;
float dy;
float signal_freq;  //set the initial speed
float st; //number of steps
float in_or_out;
float go;
float motorAxis;

float x_pos;

bool isHome;

void setup() {
  // put your setup code here, to run once:

  oled.begin();
  oled.setFont(u8g2_font_5x7_tf);    
  
  Serial.begin(115200);
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

  digitalWrite(ms1,HIGH);
  digitalWrite(ms2,HIGH);
  digitalWrite(ms3,HIGH);
  digitalWrite(dir,LOW);
  digitalWrite(slp,HIGH);
  digitalWrite(res,HIGH);

  delay(500); //initial wait for safety
  //cm.connect("J2","18611865");//change as needed.
  //cm.connect("6s08","iesc6s08");
  cm.connect("xfinity-203-03","cattle1331cover2");
  //cm.connect("EECS-MTL-RLE","");
  cm.addSlider("Speed in Hz",0,2000,1,false, &signal_freq); //add a slider called speed for axis speed
  cm.addSlider("Number of step",0,10000,1,false, &st); //number of steps
  cm.addSlider("Current Axis",0,2,1,false, &motorAxis);  //switching the axis motor
  //cm.addSlider("Desired X",0,1000,1,false, &dx); //add a slider called Desired X for movement of x axis
  //cm.addSlider("Desired Y",0,1000,1,false, &dy); //add a slider called Desired Y for movement of y axis
  cm.addToggle("Direction (back and forth)", &in_or_out);  //add toggle for the direction
  cm.addToggle("GO", &go); //add toggle for a go button
  
}

void loop() {
  //for switching platform
  switching_motor(motorAxis);
  
  digitalWrite(dir,(int) in_or_out); //writing to the direction command
  
  if (go){
    Serial.println(st,axis);
    isHome = 0;
    runComm(st,axis);
    x_pos = x_pos + st;
    go = 0;
  }
  drawtext(axisStr, signal_freq, in_or_out, isHome);

  cm.step();  
}
