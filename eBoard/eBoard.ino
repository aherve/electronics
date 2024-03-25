#define upTres 700
#define downTres 200
#define boardSize 2

int boardState[boardSize][boardSize];
int analgoReadPins[boardSize] = {A0, A1};
int digitalWritePins[boardSize] = {7, 8};

void setup() {
  for (int i = 0; i < boardSize; i++) {
    pinMode(analgoReadPins[i], INPUT);
    pinMode(digitalWritePins[i], OUTPUT);
    digitalWrite(digitalWritePins[i], LOW);
  }
  Serial.begin(9600);
}

bool readBoard() {
  int changed = false;
  for (int i = 0; i < boardSize; i++) {
    digitalWrite(digitalWritePins[i], HIGH);
    for (int j = 0; j < boardSize; j++) {
      int read = analogRead(analgoReadPins[j]);
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
    digitalWrite(digitalWritePins[i], LOW);
  }
  return changed;
}

void loop() {

  if (readBoard()) {
    display(boardState);
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
