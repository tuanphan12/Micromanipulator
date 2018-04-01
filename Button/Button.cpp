#include "Arduino.h"
#include "Button.h"

Button::Button(int pin){
    flag = 0;  
    state = 0;
    pin = p;
    t_of_state_2 = millis(); //init
    t_of_button_change = millis(); //init
    debounce_time = 10;
    long_press_time = 1000;
    button_pressed = 0;
    pinMode(pin, INPUT_PULLUP);
    _pin = pin
}

void Button::read()
{
    int button_state = digitalRead(_pin);  
    button_pressed = !button_state;
}

int Button::update() {
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