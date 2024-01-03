// serial communication with the shift register
int latchPin = 13;
int clockPin = 12;
int dataPin = 11;

int digitPins[5] = {0, 1, 2, 3, 4};

int numbers[10] = {B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111, B01111111, B01101111};

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int digitIndex = 0; digitIndex < 5; digitIndex++) {
    pinMode(digitPins[digitIndex], OUTPUT);
    digitalWrite(digitPins[digitIndex], HIGH);
  }
  int digitIndex = 0;
}
int numberToDisplay = 0;

unsigned long lastChange = millis();

void loop() {
  // count from 0 to 255 and display the number
  // on the LEDs

  unsigned long now = millis();
  if (now - lastChange > 500) {
    numberToDisplay = (numberToDisplay + 1 ) % 10;
    lastChange = now;
  }

  for (int pinIndex = 0; pinIndex < 5; pinIndex++) {

    int pin = digitPins[pinIndex];
    digitalWrite(pin, LOW);
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[numberToDisplay]);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    digitalWrite(pin, HIGH);
  }
}
