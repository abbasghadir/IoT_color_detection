#include "interrupt_manager.h"

const unsigned long debounce_time= 1000;

const int pedal_input_pin = 13;
const int reset_button_pin = 2;

unsigned long pedal_current_time = 0;
unsigned long reset_current_time = 0;

bool pedal_input_flag = false, pedal_result = false;
bool reset_button_flag = false, reset_result= false;

void IRAM_ATTR pedal_interrupt() {
    pedal_input_flag = true;
    pedal_current_time = millis();
}

void IRAM_ATTR reset_interrupt() {
    reset_button_flag = true;
    reset_current_time = millis();
}

void interrupt_init(){
    pinMode(pedal_input_pin, INPUT_PULLDOWN);
    pinMode(reset_button_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pedal_input_pin), pedal_interrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(reset_button_pin), reset_interrupt, FALLING);
}

bool pedal_interrupt_result(){
    if(digitalRead(pedal_input_pin) == LOW)
    {
        pedal_input_flag = false;
        pedal_result = false;
    }else if (millis()- pedal_current_time >= debounce_time)
    {
        pedal_result = true;
    }
    return pedal_result;
}

bool reset_interrupt_result(){
    if(digitalRead(reset_button_pin) == HIGH)
    {
        reset_button_flag = false;
        reset_result = false;
    }else if (millis()- reset_current_time >= debounce_time)
    {
        reset_result = true;
    }
    return reset_result;
}