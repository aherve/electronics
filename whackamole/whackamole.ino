#define PLAYER_RX 12
#define PLAYER_TX 11

#include <DFPlayerMini_Fast.h>
#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(PLAYER_RX, PLAYER_TX); // RX, TX
#endif

#define SOUND_INTRO 1
#define SOUND_OUTRO 2
#define SOUND_MIN 3
#define SOUND_MAX 13
DFPlayerMini_Fast player;

// serial communication with the shift register
#define SHIFT_LATCH 10
#define SHIFT_CLOCK 9
#define SHIFT_DATA 8

#define POWER_BOOTSTRAP 7

// pins that can be fed by the multiplexer. Set to low to turn on (and attach a resistor to them)
const int digitSelectors[4] = { 5,4,3,2 };  // 4 x7 segment displays
const int numberSegments[10] = { B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111, B01111111, B01101111 };

#define ledSelector A5
bool ledState[5] = {false,false,false,false,false};
bool buttonState[5] = {false,false,false,false,false};
unsigned long lastLEDChange[5] = {0,0,0,0,0};
unsigned int ledStateDuration[5] = {0,0,0,0,0};
unsigned long gameStartedAt = 0;

const int buttonInputPins[5] = {A4, A3, A2, A1, A0};
const unsigned long TIMEOUT = 30 * 1000;
const unsigned long GAME_DURATION = 10000;
unsigned int lastAction = millis();
int score = 0;

void setup() {
  pinMode(POWER_BOOTSTRAP, OUTPUT);
  digitalWrite(POWER_BOOTSTRAP, HIGH);
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
  // setup input buttons
  for (int i = 0; i < 5; i++) {
    pinMode(buttonInputPins[i], INPUT_PULLUP);
  }
  Serial.begin(115200);

#if !defined(UBRR1H)
  mySerial.begin(9600);
  player.begin(mySerial, false);
#else
  Serial1.begin(9600);
  player.begin(Serial1, false);
#endif

  randomSeed(analogRead(0));
  player.volume(8);
  delay(300);
  gameInit();
}

void loop() {
  unsigned long now = millis();
  maybeEndGame(now);
  lightUpOrDown(now);
  maybeShutDown(now);
  whack(now);
  displayNumber(score);
  displayLEDs(ledState);
}

void lightUpOrDown(unsigned long now) {
  for (int i = 0; i < 5; i++ ) {
    if(now - lastLEDChange[i] > ledStateDuration[i]) {
      ledState[i] = !ledState[i];
      lastLEDChange[i] = now;
      ledStateDuration[i] = ledState[i] ? stayOnDuration() : stayOffDuration();
    }
  }
}

void maybeEndGame(unsigned long now) {
  if ((gameStartedAt > 0) && (now - gameStartedAt > GAME_DURATION)) {
    player.play(SOUND_OUTRO);
    gameStartedAt = 0;
    // launch new game only if previous game was played;
    if (score > 0) { 
      while(millis() - now < 5000) {
        displayNumber(score);
      }
      gameInit();
    } else {
      shutDown();
    }
  }
}

void gameInit() {
  unsigned long now = millis();
  player.play(SOUND_INTRO);
  for (int i = 0; i < 5; i++) {
    ledState[i] = false;
    lastLEDChange[i] = now;
    ledStateDuration[i] = ledState[i] ? stayOnDuration() : stayOffDuration();
  } 
  score = 0;
  gameStartedAt = now;
}

int stayOnDuration() {
  return random(500,5000); // TODO: function of score
}

int stayOffDuration() {
  return random(1500, 3000); // TODO: function of score
}

uint16_t randomSound() {
  return random(SOUND_MIN, SOUND_MAX);
}

void shutDown() {
  digitalWrite(POWER_BOOTSTRAP, LOW);
  delay(10000);
}
void maybeShutDown(unsigned long now) {
  if (now - lastAction > TIMEOUT) {
    shutDown();
  }
}

void whack(unsigned long now) {
  for (int i = 0; i < 5; i++) {
    bool isPushed = digitalRead(buttonInputPins[i]) == LOW;
    if (isPushed != buttonState[i] ) {
      // change on button from up to down 
      lastAction = now;
      if (isPushed && ledState[i]) {
        player.play(randomSound());
        ledState[i] = false;
        score +=10;
      }
      buttonState[i] = isPushed;
    }
  }
}

void displayNumber(int score) {
  for (int digit = 0; digit < 4; digit++) {
    if (score == 0 && digit > 0) {
      return;
    }
    digitalWrite(SHIFT_LATCH, LOW);
    shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, numberSegments[score % 10]);
    digitalWrite(SHIFT_LATCH, HIGH);
    digitalWrite(digitSelectors[digit], LOW);
    delay(1);
    digitalWrite(digitSelectors[digit], HIGH);
    score = score / 10;
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
