/* 
 *  MIDI pedalboard with NUM_BUTTONS buttons, and one pedal.
 *  Last button is toe switch in pedal which toogles mode for pedal.
 */
 
/* In linux to listen midi, detect client with:
 * $ aconnect -i
 * then to listen
 * $ aseqdump -p 24
 * where 24 is the detected client.
 */
 
#include <MIDI.h>  // https://www.arduino.cc/reference/en/libraries/midi-library/
#define NUM_BUTTONS 9
#define CHANNEL 1  // midi channel 1=cnl1, 2=cnl2,...
#define BTN_DELAY 370  // to avoid multiple messages
#define POT_CC 100  // for exp_mode=false is this, +1 will be for exp_mode=true
#define DBNC_DELAY 30  // de bounce and 2nd btn press delay
#define SNAP_CC 69  // CC for helix snapshots

const uint8_t pedalPot = A0;  //A0 input
const uint8_t buttons[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const uint16_t maxPot = 1023;  // 1023 needs more than 8 bit (10)
const uint8_t minPot = 0;  // 0
const uint8_t ledPin = 13;

 /*
 * although calibration should change maxPot and minPot, since MIDI resolution is 0-127
 * doesn't change too much. A function could be made for calibration and change this values
 */

uint8_t intensity;  // vairable to store pedal position
uint8_t lastIntensity = 127;  // last stored pedal position, start toe
bool expMode = false;  // start false

MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){
    MIDI.begin();  // initialize MIDI
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
      pinMode(buttons[i], INPUT_PULLUP);  // initialize button pins as input with pullup internal resistors.
    pinMode(pedalPot, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void loop() {
    readButtons();
    readPedal();
}

void readButtons(){
    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        if (digitalRead(buttons[i]) == LOW){
            delay(DBNC_DELAY); // wait for other to press and de-bounce
            if (i < NUM_BUTTONS && digitalRead(buttons[i] == LOW) && digitalRead(buttons[i+1]) == LOW){
                // 2 consecutives together
                MIDI.sendControlChange(10+i, 127, CHANNEL);
                delay(BTN_DELAY);
                return;
            }
            if (i > 0 && digitalRead(buttons[i]) == LOW && digitalRead(buttons[i-1]) == LOW){
                // 2 consecutives together
                MIDI.sendControlChange(9+i, 127, CHANNEL);
                delay(BTN_DELAY);
                return;
            }
            if (digitalRead(buttons[i])==LOW){
                //only 1
                MIDI.sendControlChange(i, 127, CHANNEL);
                if (i <= 3)
                  MIDI.sendControlChange(SNAP_CC, i, CHANNEL+1);
                else
                MIDI.sendControlChange(i, 127, CHANNEL+1);
                if (i == NUM_BUTTONS-1){  // if is exp btn
                  if (expMode){
                    expMode = false;
                    digitalWrite(ledPin, LOW);
                  }
                  else{
                    expMode = true;
                    digitalWrite(ledPin, HIGH);
                  }
                }
                else
                  expMode = false;  // it's very rare to change any other parameter but leave expMode on.
                  digitalWrite(ledPin, LOW);
                delay(BTN_DELAY);
                return;
            }
        }
    }
}

void readPedal()
{
    uint8_t cc;
    intensity = (uint8_t) (map(analogRead(pedalPot), minPot, maxPot, 0, 127));
    if (intensity != lastIntensity){
      if(expMode)
        cc = POT_CC + 1;
      else
        cc = POT_CC;
      MIDI.sendControlChange(cc, intensity, CHANNEL);
      MIDI.sendControlChange(cc, intensity, CHANNEL+1);
      lastIntensity = intensity;
    }
}
