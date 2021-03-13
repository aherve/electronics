//pentatonic scale:
//  220,
//  261.6,
//   293.7,
//   329.6,
//  392,
//  440,


// Frequencies in hertz (rounded to int)
const int scale[] = {
  262,
  294,
  330,
  392,
};

// pins for LEDs
const int colors[] = {
  2,
  3,
  4,
  5,
};
const int speakerPin = 6;

// length of max melody
const int melodyLen = 500;
int melody[melodyLen]; // init melody array

int scaleLen = sizeof(scale) / sizeof(int);

int randomIndex() {
  return floor(random(scaleLen));
}

void setup() {
  randomSeed(analogRead(0));
  initMelody();
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
