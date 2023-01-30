#include <MIDI.h>  // https://www.arduino.cc/reference/en/libraries/midi-library/
#define NUM_BUTTONS  6
#define CHANNEL 1 //0=cnl1, 1=cnl2,... F=cnl16
#define BTN_DELAY 370

const uint8_t btn1 = 2;
const uint8_t btn2 = 3;
const uint8_t btn3 = 4;
const uint8_t btn4 = 5;
const uint8_t btn5 = 6;
const uint8_t btn6 = 7;

const int pedalPot = 0;  //A0 input

const uint8_t buttons[NUM_BUTTONS] = {btn1, btn2, btn3, btn4, btn5, btn6};

uint8_t intensity;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){
    MIDI.begin();
    for (int i = 0; i < NUM_BUTTONS; i++)
      pinMode(buttons[i], INPUT_PULLUP);
}

void loop() {
    readButtons();
    // readPedal();
}

void readButtons(){
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        if (digitalRead(buttons[i]) == LOW){
            delay(15); // wait for other to press and de-bounce
            if (i < NUM_BUTTONS && digitalRead(buttons[i] == LOW) && digitalRead(buttons[i+1]) == LOW){
                // 2 together
                MIDI.sendControlChange(10+i, 127, CHANNEL);
                delay(BTN_DELAY);
                return;
            }
            if (i > 0 && digitalRead(buttons[i]) == LOW && digitalRead(buttons[i-1]) == LOW){
                // 2 together
                MIDI.sendControlChange(9+i, 127, CHANNEL);
                delay(BTN_DELAY);
                return;
            }
            if (digitalRead(buttons[i])==LOW){
                //only 1
                MIDI.sendControlChange(i, 127, CHANNEL);
                delay(BTN_DELAY);
                return;
            }
        }
    }
}

/*
void readPedal()
{
  int val = analogRead(pedalPot);
  intensity = (uint8_t) (map(val, 0, 1023, 0, 127));
}
*/
