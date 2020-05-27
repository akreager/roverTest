//See BareMinimum example for a list of library functions

//Includes required to use Roboclaw library
#include "RoboClaw.h"

RoboClaw roboclaw(&Serial1, 10000);

#define address 0x80

//Serial LCD commands
#define COMMAND 0xFE
#define CLEAR   0x01
#define LINE0   0x80
#define LINE1   0xC0

void setup() {
  //Communciate with roboclaw at 38400 baud
  roboclaw.begin(38400);
  delay(100);

  //Communicate with serial LCD at 9600 baud
  Serial2.begin(9600);
  delay(100);
  Serial2.write(COMMAND);
  Serial2.write(CLEAR);
  delay(100);
}

void loop() {
  char version[32];

  if (roboclaw.ReadVersion(address, version)) {
    Serial2.write(COMMAND);
    Serial2.write(LINE0);
    Serial2.print(version);
  }

  delay(100);
}
