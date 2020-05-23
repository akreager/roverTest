//Sketch to read data from Roboclaw motordriver
//Basicmicro Arduino library and examples:
//https://github.com/basicmicro/roboclaw_arduino_library
//Connections:
//Roboclaw     Arduino Mega     Serial LCD
//   S1          18, TX1
//   S2          19, RX1
//               16, TX2           RX

#include "RoboClaw.h"

RoboClaw roboclaw(&Serial1, 10000);

#define address 0x80

//Serial LCD commands
#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0

int led = 13;
unsigned int rawBattery = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  //Communciate with roboclaw at 230400 baud
  roboclaw.begin(230400);
  delay(100);

  //Communicate with serial LCD at 9600 baud
  Serial2.begin(9600);
  clear_lcd();
  delay(100);

  Serial2.write(COMMAND);
  Serial2.write(LINE0 + 2);
  Serial2.print(F("Main Battery:"));

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool valid1;

  rawBattery = roboclaw.ReadMainBatteryVoltage(address, &valid1);
  int mainBattery = rawBattery / 10;

  if (valid1) {
    Serial2.write(COMMAND);
    Serial2.write(LINE1 + 5);
    Serial2.print(mainBattery);
    Serial2.print(F("."));
    Serial2.print(rawBattery - (mainBattery * 10));
    Serial2.print(F(" V   "));
  }
  else {
    Serial2.write(COMMAND);
    Serial2.write(LINE1 + 4);
    Serial2.print(F("INVALID! "));
  }

  delay(1000);
}

void clear_lcd(void)
{
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
}
