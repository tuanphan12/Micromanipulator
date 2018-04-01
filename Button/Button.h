/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

class Morse
{
    public:
        unsigned long t_of_state_2;
        unsigned long t_of_button_change;    
        unsigned long debounce_time;
        unsigned long long_press_time;
        int pin;
        int flag;
        bool button_pressed;
        int state; // This is public for the sake of convenience
};

#endif