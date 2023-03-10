#include <MIDI.h>  // https://www.arduino.cc/reference/en/libraries/midi-library/
#define NUM_BUTTONS  6
#define CHANNEL 1 //1=cnl1, 2=cnl2,...
#define BTN_DELAY 370
#define potCCNumber 100

const uint8_t btn1 = 2;
const uint8_t btn2 = 3;
const uint8_t btn3 = 4;
const uint8_t btn4 = 5;
const uint8_t btn5 = 6;
const uint8_t btn6 = 7;

const int pedalPot = 0;  //A0 input

const uint8_t buttons[NUM_BUTTONS] = {btn1, btn2, btn3, btn4, btn5, btn6};

uint8_t intensity;
uint8_t lastIntensity = 0;
const int maxPot = 1010;  //1023
const int minPot = 10;  //0

MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){
    MIDI.begin();
    for (int i = 0; i < NUM_BUTTONS; i++)
      pinMode(buttons[i], INPUT_PULLUP);
}

void loop() {
    readButtons();
    readPedal();
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

void readPedal()
{
    int val = analogRead(pedalPot);
    intensity = (uint8_t) (map(val, minPot, maxPot, 0, 127));
    if (intensity != lastIntensity){
        MIDI.sendControlChange(potCCNumber, intensity, CHANNEL);
        lastIntensity = intensity;
      }
}
