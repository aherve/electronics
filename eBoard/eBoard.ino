#include <SPI.h>

#define upTres 500
#define downTres 470
#define boardSize 2
#define ledLatchPin 4
#define hallLatchPin 5

int boardState[boardSize][boardSize];
bool ledState[boardSize][boardSize] = {
  { true, false },
  { false, true }
};
int const readPins[boardSize]{ A0, A1 };

void setup() {
  for (int i = 0; i < boardSize; i++) {
    pinMode(readPins[i], INPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(ledLatchPin, OUTPUT);
    pinMode(hallLatchPin, OUTPUT);
    digitalWrite(hallLatchPin, LOW);
    digitalWrite(ledLatchPin, LOW);
    digitalWrite(clockPin, LOW);
  }
  SPI.begin();
  Serial.begin(9600);
  Serial.println("Hello");
  displayBoard();
}

bool readBoard() {
  int changed = false;
  byte iByte = 1;
  for (byte i = 0; i < boardSize; i++) {
    SPI.transfer(iByte);
    digitalWrite(hallLatchPin, HIGH);
    delay(1);
    digitalWrite(hallLatchPin, LOW);
    for (byte j = 0; j < boardSize; j++) {
      int read = analogRead(readPins[j]);
      if (read < downTres) {
        read = -1;
      } else if (read > upTres) {
        read = 1;
      } else {
        read = 0;
      }
      if (read != boardState[i][j]) {
        changed = true;
        boardState[i][j] = read;
      }
    }
    iByte <<= 1;
  }
  return changed;
}

void displayLEDs() {
  byte iByte = 1;
  for (byte i = 0; i < boardSize; i++) {
    bool hasValue = false;
    for (byte j = 0; j < boardSize; j++) {
      if (ledState[i][j]) {
        hasValue = true;
        break;
      }
    }
    if (!hasValue) {
      iByte <<= 1;
      continue;
    }

    byte jByte = 0;
    for (byte j = 0; j < boardSize; j++) {
      jByte <<= 1;
      if (!ledState[i][j]) {  // 0 to enable
        jByte++;
      }
    }

    SPI.transfer(jByte);
    SPI.transfer(iByte);
    SPI.transfer(0);

    digitalWrite(ledLatchPin, HIGH);
    digitalWrite(ledLatchPin, LOW);
    delay(2);

    iByte <<= 1;
  }
}

unsigned long lastLedChange = 0;
void loop() {

  if (readBoard()) {
    displayBoard();
  }

  if ((millis() - lastLedChange) > 1000) {
    for (byte i = 0; i < boardSize; i++) {
      for (byte j = 0; j < boardSize; j++) {
        ledState[i][j] = !ledState[i][j];
      }
    }
    lastLedChange = millis();
  }

  displayLEDs();
}

void displayBoard() {
  Serial.println("--------");
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      Serial.print(boardState[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}