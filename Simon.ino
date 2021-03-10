/*
  Adafruit Arduino - Lesson 10. Simple Sounds
*/

//pentatonic scale:
//  220,
//  261.6,
//   293.7,
//   329.6,
//  392,
//  440,


int speakerPin = 2;

int greenIn = 3;
int blueIn = 4;
int yellowIn = 5;
int redIn = 6;

int greenOut = 3;
int blueOut = 4;
int yellowOut = 5;
int redOut = 6;

int input = 0;

int scale[] = {
  262,
  294,
  330,
  392,
};

int colorsIn[] = {
  greenIn,
  blueIn,
  yellowIn,
  redIn,
};

int colorsOut[] = {
  greenOut,
  blueOut,
  yellowOut,
  redOut,
};

int scaleLen = sizeof(scale) / sizeof(int);

int melodyLen = 7;

int randomIndex(int scale[], int len) {
  return floor(random(len));
}

void setup() {
  randomSeed(analogRead(0));

  for (int i = 0; i < melodyLen; i++)
  {
    play(randomIndex(scale, scaleLen));
  }
  noTone(speakerPin);
}

void play(int i) {
  digitalWrite(colorsOut[i], HIGH);
  tone(speakerPin, scale[i]);
  delay(200);
  noTone(speakerPin);
  digitalWrite(colorsOut[i], LOW);
  delay(100);
}

void loop() {

  for (int i = 0; i < scaleLen; i++) {
    input = digitalRead(colorsIn[i]);
    if (input == HIGH) {
      play(i);
    }
  }


}
