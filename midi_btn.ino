#define NUM_BUTTONS  6
#define CHANNEL 1
#define BTN_DELAY 370
#define DEBUG 1

const uint8_t btn1 = 2;
const uint8_t btn2 = 3;
const uint8_t btn3 = 4;
const uint8_t btn4 = 5;
const uint8_t btn5 = 6;
const uint8_t btn6 = 7;

const int pedalPot = 0;  //A0 input

const uint8_t buttons[NUM_BUTTONS] = {btn1, btn2, btn3, btn4, btn5, btn6};

uint8_t intensity;

void setup(){
    if DEBUG
        SERIAL_PORT_RATE = 9600;  //for serial monitor
    else
        SERIAL_PORT_RATE = 31250;  // for MIDI
    Serial.begin(SERIAL_PORT_RATE); // setup serial for text
    for (int i = 0; i < NUM_BUTTONS; i++)
      pinMode(buttons[i], INPUT_PULLUP);
}

void loop() {
    readButtons();
    // readPedal();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte control, byte value=127){
    if DEBUG
      Serial.println(control); // log result
    else
}

void readButtons(){
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        if (digitalRead(buttons[i]) == LOW){
            delay(15); // wait for other to press and de-bounce
            if (i < NUM_BUTTONS && digitalRead(buttons[i] == LOW) && digitalRead(buttons[i+1]) == LOW){
                // 2 together
                controlChange(10+i, 127);
                delay(BTN_DELAY);
                return;
            }
            if (i > 0 && digitalRead(buttons[i]) == LOW && digitalRead(buttons[i-1]) == LOW){
                // 2 together
                controlChange(9+i, 127);
                delay(BTN_DELAY);
                return;
            }
            if (digitalRead(buttons[i])==LOW){
                //only 1
                controlChange(i, 127);
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
