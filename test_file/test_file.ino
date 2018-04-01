#include <SPI.h>

#include <U8g2lib.h>

#include <Button.h>

const int BUTTON_PIN = 4;

//class Button{
//  public:
//  unsigned long t_of_state_2;
//  unsigned long t_of_button_change;    
//  unsigned long debounce_time;
//  unsigned long long_press_time;
//  int pin;
//  int flag;
//  bool button_pressed;
//  int state; // This is public for the sake of convenience
//  Button(int p) {
//    flag = 0;  
//    state = 0;
//    pin = p;
//    t_of_state_2 = millis(); //init
//    t_of_button_change = millis(); //init
//    debounce_time = 10;
//    long_press_time = 1000;
//    button_pressed = 0;
//  }
//  void read() {
//    int button_state = digitalRead(pin);  
//    button_pressed = !button_state;
//  }
//  
//  int update() {
//    read();
//    flag = 0;
//    if (state==0) { // Unpressed, rest state
//      if (button_pressed) {
//        state = 1;
//        t_of_button_change = millis();
//      }
//    } else if (state==1) { //Tentative pressed
//      if (!button_pressed) {
//        state = 0;
//        t_of_button_change = millis();
//      } else if (millis()-t_of_button_change >= debounce_time) {
//        state = 2;
//        t_of_state_2 = millis();
//      }
//    } else if (state==2) { // Short press
//      if (!button_pressed) {
//        state = 4;
//        t_of_button_change = millis();
//      } else if (millis()-t_of_state_2 >= long_press_time) {
//        state = 3;
//      }
//    } else if (state==3) { //Long press
//      if (!button_pressed) {
//        state = 4;
//        t_of_button_change = millis();
//      }
//    } else if (state==4) { //Tentative unpressed
//      if (button_pressed && millis()-t_of_state_2 < long_press_time) {
//        state = 2; // Unpress was temporary, return to short press
//        t_of_button_change = millis();
//      } else if (button_pressed && millis()-t_of_state_2 >= long_press_time) {
//        state = 3; // Unpress was temporary, return to long press
//        t_of_button_change = millis();
//      } else if (millis()-t_of_button_change >= debounce_time) { // A full button push is complete
//        state = 0;
//        if (millis()-t_of_state_2 < long_press_time) { // It is a short press
//          flag = 1;
//        } else {  // It is a long press
//          flag = 2;
//        }
//      }
//    }
//    return flag;
//  }
//};

Button button(BUTTON_PIN);
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled(U8G2_R0, 5, 17, 16);

void setup() {
  Serial.begin(115200);               // Set up serial port
  oled.begin();
  oled.setFont(u8g2_font_ncenB10_tr);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}
 
//delays are used here just for testing purposes!
void loop() {
  oled.clearBuffer();
  oled.setCursor(0, 15);
  int flag = button.update();
  Serial.println(flag);
  if (flag==1) {
    oled.print("short press");
    oled.sendBuffer();
  } else if (flag==2) {
    oled.print("long press");
    oled.sendBuffer();   
  } else {
      oled.print(button.state);
      oled.sendBuffer();
      //delay(1);
  }
}
