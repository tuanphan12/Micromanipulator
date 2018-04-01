#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
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

        Button(int pin);
        void read();
        int update();
    private:
        int _pin;
};

#endif