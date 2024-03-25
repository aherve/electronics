void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  Serial.begin(9600);
  Serial.println('hello');
}

int q0 = 3;
int q1 = 3;
int q2 = 3;
int q3 = 3;
int read;
void loop() {

  digitalWrite(7, HIGH);
  read = analogRead(A0);
  Serial.println(read);

  if (read < 200 && q0 != 0) {
    q0 = 0;
    display();
  } else if (read < 700 && read > 200 && q0 != 1) {
    q0 = 1;
    display();
  } else if (read > 700 && q0 != 2) {
    q0 = 2;
    display();
  }
  read = analogRead(A1);

  if (read < 200 && q1 != 0) {
    q1 = 0;
    display();
  } else if (read < 700 && read > 200 && q1 != 1) {
    q1 = 1;
    display();
  } else if (read > 700 && q1 != 2) {
    q1 = 2;
    display();
  }
  digitalWrite(7, LOW);

  digitalWrite(8, HIGH);
  read = analogRead(A0);

  if (read < 200 && q2 != 0) {
    q2 = 0;
    display();
  } else if (read < 700 && read > 200 && q2 != 1) {
    q2 = 1;
    display();
  } else if (read > 700 && q2 != 2) {
    q2 = 2;
    display();
  }
  read = analogRead(A1);

  if (read < 200 && q3 != 0) {
    q3 = 0;
    display();
  } else if (read < 700 && read > 200 && q3 != 1) {
    q3 = 1;
    display();
  } else if (read > 700 && q3 != 2) {
    q3 = 2;
    display();
  }
  digitalWrite(8, LOW);
}

void display() {
  Serial.print(q0);
  Serial.print(", ");
  Serial.print(q1);
  Serial.print(", ");
  Serial.print(q2);
  Serial.print(", ");
  Serial.print(q3);
  Serial.print("\n");
}
