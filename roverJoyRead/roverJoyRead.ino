//Basicmicro Arduino library and examples:
//https://github.com/basicmicro/roboclaw_arduino_library

#include "RoboClaw.h"

RoboClaw roboclaw(&Serial1, 10000);

#define address 0x80

//Serial LCD commands
#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0

unsigned int rawBattery = 0;
unsigned int rawTemp1 = 0;
unsigned int rawTemp2 = 0;
int rawCurrentMotor1 = 0;
int rawCurrentMotor2 = 0;
int vrX = A0;
int vrY = A1;
int valX = 64;
int valY = 64;
int readInterval = 0;
int readThreshold = 10;

void setup() {
  // put your setup code here, to run once:
  //debug serial
  Serial.begin(9600);

  //Communciate with roboclaw at 38400 baud
  roboclaw.begin(38400);
  delay(100);
  roboclaw.ForwardBackwardMixed(address, valY);
  roboclaw.LeftRightMixed(address, valX);
  delay(100);

  //Communicate with serial LCD at 9600 baud
  Serial2.begin(9600);
  clearLCD();
  delay(100);

}

void loop() {
  // put your main code here, to run repeatedly:
  readInterval++;

  valX = map(analogRead(vrX), 0, 1023, 0, 127);
  valY = map(analogRead(vrY), 0, 1023, 127, 0);

  roboclaw.ForwardBackwardMixed(address, valY);
  roboclaw.LeftRightMixed(address, valX);

  if (readInterval >= readThreshold) {
    updateLCD1();
    readInterval = 0;
  }
}

void clearLCD(void)
{
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
}

void updateLCD0(void)
{
  bool valid1;

  //Read data fromm roboclaw
  rawBattery = roboclaw.ReadMainBatteryVoltage(address, &valid1);
  if (roboclaw.ReadTemp(address, rawTemp1)) {};
  if (roboclaw.ReadTemp2(address, rawTemp2)) {};
  int mainBattery = rawBattery / 10;
  int temp1 = rawTemp1 / 10;
  int temp2 = rawTemp2 / 10;

  //Display main voltage
  if (valid1) {
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(mainBattery);
    Serial2.print(F("."));
    Serial2.print(rawBattery - (mainBattery * 10));
    Serial2.print(F(" V   "));
  }
  else {
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(F("INV  "));
  }

  //Display board temperature 1
  Serial2.write(COMMAND);
  Serial2.write(LINE0 + 8);
  Serial2.print(temp1);
  Serial2.print(F("."));
  Serial2.print(rawTemp1 - (temp1 * 10));
  Serial2.print((char)223);
  Serial2.print("C   ");

  //Display board temperature 2
  Serial2.write(COMMAND);
  Serial2.write(LINE1 + 8);
  Serial2.print(temp2);
  Serial2.print(F("."));
  Serial2.print(rawTemp2 - (temp2 * 10));
  Serial2.print((char)223);
  Serial2.print("C  ");
}

void updateLCD1(void)
{
  bool valid1;

  //Read data fromm roboclaw
  rawBattery = roboclaw.ReadMainBatteryVoltage(address, &valid1);
  if (roboclaw.ReadTemp(address, rawTemp1)) {};
  if (roboclaw.ReadCurrents(address, rawCurrentMotor1, rawCurrentMotor2)) {};
  int mainBattery = rawBattery / 10;
  int temp1 = rawTemp1 / 10;
  int currentM1 = rawCurrentMotor1 / 100;
  int currentM2 = rawCurrentMotor2 / 100;

  //Display main voltage
  if (valid1) {
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(mainBattery);
    Serial2.print(F("."));
    Serial2.print(rawBattery - (mainBattery * 10));
    Serial2.print(F("V   "));
  }
  else {
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(F("INV  "));
  }

  //Display board temperature 1
  Serial2.write(COMMAND);
  Serial2.write(LINE0 + 8);
  Serial2.print(temp1);
  Serial2.print(F("."));
  Serial2.print(rawTemp1 - (temp1 * 10));
  Serial2.print((char)223);
  Serial2.print("C   ");

  //Display Motor 1 current
  Serial2.write(COMMAND);
  Serial2.write(LINE1);
  Serial2.print(currentM1);
  Serial2.print(F("."));
  Serial2.print(rawCurrentMotor1 - (currentM1 * 100));
  Serial2.print(F("A  "));

  //Display Motor 2 current
  Serial2.write(COMMAND);
  Serial2.write(LINE1 + 8);
  Serial2.print(currentM2);
  Serial2.print(F("."));
  Serial2.print(rawCurrentMotor2 - (currentM2 * 100));
  Serial2.print(F("A  "));
}
