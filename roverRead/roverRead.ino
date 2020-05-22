#include "RoboClaw.h"

RoboClaw roboclaw(&Serial1, 10000);

#define address 0x80

#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0

int led = 13;

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
  Serial2.write(LINE0);
  Serial2.print(F(" Main Battery:"));

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool valid1;
  uint16_t rawBattery = roboclaw.ReadMainBatteryVoltage(address, &valid1);
  //uint16_t rawBattery = roboclaw.ReadMainBatteryVoltage(address);
  float mainBattery = rawBattery / 10.0;

  if (valid1) {
    Serial2.write(COMMAND);
    Serial2.write(LINE1);
    Serial2.print(mainBattery);
    Serial2.print(F(" V    "));
  }
  else {
    Serial2.write(COMMAND);
    Serial2.write(LINE1);
    Serial2.print(F("    INVALID! "));
  }
  delay(1000);
}

void clear_lcd(void)
{
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
}
