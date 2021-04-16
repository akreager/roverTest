int vrX = A0;
int vrY = A1;
int valX = 64;
int valY = 64;

int readInterval = 0;
int readThreshold = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  Serial.println("X,Y");

  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
    valX = map(analogRead(vrX), 0, 1023, 0, 127);
    valY = map(analogRead(vrY), 0, 1023, 127, 0);
    Serial.print(valX);
    Serial.print(",");
    Serial.println(valY);
}
