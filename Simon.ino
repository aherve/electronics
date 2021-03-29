#define displayPinSize 8 // 7-segments ( + dot ) display
// scale:
//  220,
//  261.6,
//   293.7,
//   329.6,
//  392,
//  440,


// Frequencies in hertz (rounded to int)
const int scale[] = {
  220,
  262,
  330,
  294,
};

// pins for LEDs
const int colors[] = {
  2,
  3,
  4,
  5,
};
const int speakerPin = 6;
const int gameModePin = 7;

// length of max melody
const int melodyLen = 500;
int melody[melodyLen]; // init melody array

int scaleLen = sizeof(scale) / sizeof(int);

// pins for score display
const int displayPins[displayPinSize] = {A0, A1, A2, A3, A4, A5, 8, 9}; // A6 and A7 are broken it seems

int randomIndex() {
  return floor(random(scaleLen));
}

// 3-way switch can power up the arduino board, while choosing between two game modes
bool challengeMode = false;

void setup() {

  // Set display displayPins as OUTPUT
  for (int i = 0; i < displayPinSize; i++) {
    pinMode(displayPins[i], OUTPUT);
  }

  pinMode(challengeMode, INPUT);
  challengeMode = digitalRead(gameModePin) == HIGH;
  randomSeed(analogRead(0));
  if ( challengeMode ) {
    initMelody();
  }
  blink();
}

void play(int i) {
  // take control of led pin
  pinMode(colors[i], OUTPUT);
  // light LED
  digitalWrite(colors[i], HIGH);

  // play music
  tone(speakerPin, scale[i]);
  delay(200);
  noTone(speakerPin);

  // turn off LED
  digitalWrite(colors[i], LOW);
  delay(100);
  // give back led pin for reading
  pinMode(colors[i], INPUT);
}

int level = 0;
int cursor = 0;
void loop() {
  // free mode: just play notes
  while ( !challengeMode ) {
    getInput();
  }
  
  display(level);
  level++;
  playUpTo(level);

  cursor = 0;
  while (cursor < level) {
    if (getInput() == melody[cursor]) {
      cursor++;
      if (cursor == level) {
        delay(800);
      }
    } else {
      fail();
      delay(500);
      break;
    }
  }
}

void fail() {
  blink();
  initMelody();
  cursor = 0;
  level = 0;
}

int getInput() {
  while(true) {
    for (int i = 0; i < scaleLen; i++) {
      if (digitalRead(colors[i])== HIGH) { 
        play(i);
        return i;
      }
    }
  }
}

void playUpTo(int iMax) {
  for (int i = 0; i < iMax ; i++) {
    play(melody[i]);
  }
}

void initMelody() {
  for (int i = 0; i < melodyLen; i++) {
    melody[i] = randomIndex();
  }
}

void blink() {
  for (int i = 0; i < scaleLen; i++) {
    pinMode(colors[i], OUTPUT);
  }

  for (int b = 0; b < 3; b++) {
    for (int i = 0; i < scaleLen; i++) {
      digitalWrite(colors[i], HIGH);
    }
    delay(300);
    for (int i = 0; i < scaleLen; i++) {
      digitalWrite(colors[i], LOW);
    }
    delay(300);
  }

  for (int i = 0; i < scaleLen; i++) {
    pinMode(colors[i], INPUT);
  }
}

void display(int i) {
  int modTen = i % 10;

  // A
  if( modTen == 1 || modTen == 4 ) {
    digitalWrite(displayPins[0], LOW);
  } else {
    digitalWrite(displayPins[0], HIGH);
  }
  // B
  if( modTen == 5 || modTen == 6 ) {
    digitalWrite(displayPins[1], LOW);
  } else {
    digitalWrite(displayPins[1], HIGH);
  }

  // C
  if( modTen == 2 ) {
    digitalWrite(displayPins[2], LOW);
  } else {
    digitalWrite(displayPins[2], HIGH);
  }

  // D
  if( modTen == 1 || modTen == 4 || modTen == 7 ) {
    digitalWrite(displayPins[3], LOW);
  } else {
    digitalWrite(displayPins[3], HIGH);
  }

  // E
  if( modTen == 1 || modTen == 3 || modTen == 4 || modTen == 5 || modTen == 7 || modTen == 9 ) {
    digitalWrite(displayPins[4], LOW);
  } else {
    digitalWrite(displayPins[4], HIGH);
  }

  // F
  if( modTen == 1 || modTen == 2 || modTen == 3 || modTen == 7  ) {
    digitalWrite(displayPins[5], LOW);
  } else {
    digitalWrite(displayPins[5], HIGH);
  }

  // G
  if( modTen == 1 || modTen == 7 || modTen == 0  ) {
    digitalWrite(displayPins[6], LOW);
  } else {
    digitalWrite(displayPins[6], HIGH);
  }

  // dot
  if (i > 9) {
    digitalWrite(displayPins[7], HIGH);
  } else {
    digitalWrite(displayPins[7], LOW);
  }
}
