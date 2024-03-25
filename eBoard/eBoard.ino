#define upTres 500
#define downTres 470
#define boardSize 2
#define dataPin 2
#define clockPin 3
#define ledLatchPin 4
#define hallLatchPin 5


int boardState[boardSize][boardSize];
bool ledState[boardSize][boardSize] = {
  {true, false},
  {false, true}
};
int const readPins[boardSize] {A0, A1};

void setup() {
  for (int i = 0; i < boardSize; i++) {
    pinMode(readPins[i], INPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(ledLatchPin, OUTPUT);
    pinMode(hallLatchPin, OUTPUT);
  }
  Serial.begin(9600);
}

bool readBoard() {
  int changed = false;
  int shiftNumber = 1;
  for (int i = 0; i < boardSize; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, shiftNumber);
    digitalWrite(hallLatchPin, HIGH);
    delay(1);
    digitalWrite(hallLatchPin, LOW);
    for (int j = 0; j < boardSize; j++) {
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
    shiftNumber = shiftNumber << 1;
  }
  return changed;
}

void displayLEDs() {
  int iByte = 1;
  for (int i = 0; i < boardSize; i++) {
    bool hasValue = false;
    for (int j = 0; j < boardSize; j++) {
      if (ledState[i][j]) {
        hasValue = true;
        break;
      }
    }
    if (!hasValue) { continue; }

    int jByte = 0;
    for (int j = 0; j < boardSize; j++) {
      jByte << 1;
      if (ledState[i][j]) {
        jByte++;
      }
    }

    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, iByte);
    shiftOut(dataPin, clockPin, MSBFIRST, jByte);
    digitalWrite(ledLatchPin, HIGH);
    delay(10);
    digitalWrite(ledLatchPin, LOW);

    iByte << 1;
  }
}

unsigned long lastLedChange = 0;
void loop() {

  if (readBoard()) {
    display(boardState);
  }

  if ((millis() - lastLedChange) > 1000) {
    for (int i = 0; i < boardSize; i++) {
      for (int j = 0; j < boardSize; j++) {
        ledState[i][j] = !ledState[i][j];
      }
    }
  }

}

void display(int boardState[boardSize][boardSize]) {
  Serial.println("--------");
  for (int i =0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      Serial.print(boardState[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
