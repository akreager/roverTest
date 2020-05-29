//See BareMinimum example for a list of library functions

//Includes required to use Roboclaw library
#include "RoboClaw.h"

RoboClaw roboclaw(&Serial1, 10000);

#define address 0x80

int vrX = A0;
int vrY = A1;
int valX = 64;
int valY = 64;

int led = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);

  //Serial.begin(230400);
  //Serial.println("X,Y");

  //Communciate with roboclaw at 38400 baud
  roboclaw.begin(38400);
  roboclaw.ForwardBackwardMixed(address, valY);
  roboclaw.LeftRightMixed(address, valX);

  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  valX = map(analogRead(vrX), 0, 1023, 0, 127);
  valY = map(analogRead(vrY), 0, 1023, 127, 0);

  roboclaw.ForwardBackwardMixed(address, valY);
  roboclaw.LeftRightMixed(address, valX);
}
