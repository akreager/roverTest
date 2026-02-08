//Sketch to read data from Roboclaw motordriver
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

void setup() {
  // put your setup code here, to run once:
  //Communciate with roboclaw at 38400 baud
  roboclaw.begin(38400);
  delay(100);

  //Communicate with serial LCD at 9600 baud
  Serial2.begin(9600);
  clear_lcd();
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool valid1;

  //Read data fromm roboclaw
  rawBattery = roboclaw.ReadMainBatteryVoltage(address, &valid1);
  if (roboclaw.ReadTemp(address, rawTemp1)) {};
  if (roboclaw.ReadTemp2(address, rawTemp2)) {};
  int mainBattery = rawBattery / 10;
  int Temp1 = rawTemp1 / 10;
  int Temp2 = rawTemp2 / 10;

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
  Serial2.print(Temp1);
  Serial2.print(F("."));
  Serial2.print(rawTemp1 - (Temp1 * 10));
  Serial2.print((char)223);
  Serial2.print("C   ");

  //Display board temperature 2
  Serial2.write(COMMAND);
  Serial2.write(LINE1 + 8);
  Serial2.print(Temp2);
  Serial2.print(F("."));
  Serial2.print(rawTemp2 - (Temp2 * 10));
  Serial2.print((char)223);
  Serial2.print("C  ");

  delay(1000);
  //clear_lcd();
  //delay(100);
}

void clear_lcd(void)
{
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
}
