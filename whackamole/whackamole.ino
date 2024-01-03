// serial communication with the shift register
#define SHIFT_LATCH 13
#define SHIFT_CLOCK 12
#define SHIFT_DATA 11

// pins that can be fed by the multiplexer. Set to low to turn on (and attach a resistor to them)
const int digitSelectors[4] = { 0, 1, 2, 3 };  // 4 x7 segment displays
const int numberSegments[10] = { B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111, B01111111, B01101111 };

#define ledSelector 4
bool ledState[5] = {true,false,true,false,true};

void setup() {
  //set pins to output so you can control the shift register
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(SHIFT_CLOCK, OUTPUT);
  pinMode(SHIFT_DATA, OUTPUT);
  // set the selector pins to output, and set to HIGH (off)
  pinMode(ledSelector, OUTPUT);
  digitalWrite(ledSelector, HIGH);
  // set the digit pins to output, and set their selector to HIGH (off)
  for (int digitIndex = 0; digitIndex < 5; digitIndex++) {
    pinMode(digitSelectors[digitIndex], OUTPUT);
    digitalWrite(digitSelectors[digitIndex], HIGH);
  }
}

unsigned long lastChange = millis();
int value = 0;
void loop() {
  unsigned long now = millis();
  if (now - lastChange > 500) {
    value++;
    lastChange = now;

    for (int i = 0; i < 5; i++) {
      ledState[i] = !ledState[i];
    }

  }

  displayNumber(value);
  displayLEDs(ledState);
}

void displayNumber(int value) {
  for (int digit = 0; digit < 4; digit++) {
    if (value == 0 && digit > 0) {
      return;
    }
    digitalWrite(SHIFT_LATCH, LOW);
    shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, numberSegments[value % 10]);
    digitalWrite(SHIFT_LATCH, HIGH);
    digitalWrite(digitSelectors[digit], LOW);
    delay(1);
    digitalWrite(digitSelectors[digit], HIGH);
    value = value / 10;
  }
}

void displayLEDs(bool state[5]) {
  int asNumber = 0;
  for (int i = 0; i < 5; i++) {
    asNumber = asNumber << 1;
    if (state[i]) {
      asNumber++;
    }
  }
    digitalWrite(SHIFT_LATCH, LOW);
    shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, asNumber);
    digitalWrite(SHIFT_LATCH, HIGH);
    digitalWrite(ledSelector, LOW);
    delay(10);
    digitalWrite(ledSelector, HIGH);
}
