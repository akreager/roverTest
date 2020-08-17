//See BareMinimum example for a list of library functions

//Includes required to use Roboclaw library
#include "RoboClaw.h"

RoboClaw roboclaw(&Serial1, 10000);

#define address 0x80

int var1 = 64;
int var2 = 64;

int led = 13;

void setup() {
  //Communciate with roboclaw at 230400 baud
  roboclaw.begin(230400);
  roboclaw.ForwardBackwardMixed(address, var1);
  roboclaw.LeftRightMixed(address, var2);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  delay(500);
}

void loop() {
  //Accelerate forward from stop to full speed
  while (var1 > 0) {
    roboclaw.ForwardBackwardMixed(address, var1);
    delay(100);
    var1--;
  }
  roboclaw.ForwardBackwardMixed(address, var1);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Decellerate forward from full speed to stop
  while (var1 < 64) {
    roboclaw.ForwardBackwardMixed(address, var1);
    delay(100);
    var1++;
  }
  roboclaw.ForwardBackwardMixed(address, var1);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Accelerate reverse from stop to full speed
  while (var1 < 127) {
    roboclaw.ForwardBackwardMixed(address, var1);
    delay(100);
    var1++;
  }
  roboclaw.ForwardBackwardMixed(address, var1);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Decelerate reverse from full speed to stop
  while (var1 > 64) {
    roboclaw.ForwardBackwardMixed(address, var1);
    delay(100);
    var1--;
  }
  roboclaw.ForwardBackwardMixed(address, var1);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Accelerate spin right from stop to full speed
  while (var2 > 0) {
    roboclaw.LeftRightMixed(address, var2);
    delay(100);
    var2--;
  }
  roboclaw.LeftRightMixed(address, var2);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Decelerate spin right from full speed to stop
  while (var2 < 64) {
    roboclaw.LeftRightMixed(address, var2);
    delay(100);
    var2++;
  }
  roboclaw.LeftRightMixed(address, var2);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Accelerate spin left from stop to full speed
  while (var2 < 127) {
    roboclaw.LeftRightMixed(address, var2);
    delay(100);
    var2++;
  }
  roboclaw.LeftRightMixed(address, var2);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Decelerate spin left from full speed to stop
  while (var2 > 64) {
    roboclaw.LeftRightMixed(address, var2);
    delay(100);
    var2--;
  }
  roboclaw.LeftRightMixed(address, var2);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Accelerate forward from stop to full speed
  while (var1 > 0) {
    roboclaw.ForwardBackwardMixed(address, var1);
    delay(100);
    var1--;
  }
  roboclaw.ForwardBackwardMixed(address, var1);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Accelerate spin right from stop to full speed
  while (var2 > 0) {
    roboclaw.LeftRightMixed(address, var2);
    delay(100);
    var2--;
  }
  roboclaw.LeftRightMixed(address, var2);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Decelerate spin right from full speed to stop
  while (var2 < 64) {
    roboclaw.LeftRightMixed(address, var2);
    delay(100);
    var2++;
  }
  roboclaw.LeftRightMixed(address, var2);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);

  //Decellerate forward from full speed to stop
  while (var1 < 64) {
    roboclaw.ForwardBackwardMixed(address, var1);
    delay(100);
    var1++;
  }
  roboclaw.ForwardBackwardMixed(address, var1);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);
}

//error conditions
void error01() {
  while (1) {
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(2000);
  }
}

void error02() {
  while (1) {
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(2000);
  }
}

void error03() {
  while (1) {
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(2000);
  }
}

void error04() {
  while (1) {
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(2000);
  }
}
