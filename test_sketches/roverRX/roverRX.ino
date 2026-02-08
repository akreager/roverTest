/*
  Optimized fork of nRF24L01 for Arduino & Raspberry Pi/Linux Devices:
  TMRh20/RF24, https://github.com/tmrh20/RF24/
  nRF24/RF24 is licensed under the
  GNU General Public License v2.0

  nRF24 examples used:
  https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
  https://howtomechatronics.com/projects/diy-arduino-rc-transmitter/

  Basicmicro Roboclaw Arduino library and examples:
  https://github.com/basicmicro/roboclaw_arduino_library
*/

//Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RoboClaw.h>

//nRF24 connected to hardware SPI at address 00001
#define CE_PIN 49
#define CSN_PIN 53
RF24 radio(CE_PIN, CSN_PIN);
const byte nRFaddress[6] = "00001";

//Roboclaw connected to Serial1 at address 0x80
RoboClaw roboclaw(&Serial1, 10000);
#define rcAddress 0x80

unsigned int rawBattery = 0;
unsigned int rawTemp1 = 0;
unsigned int rawTemp2 = 0;
int rawCurrentMotor1 = 0;
int rawCurrentMotor2 = 0;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int readInterval = 0;
int readThreshold = 10;

//package to contain incoming data
struct dataPackage {
  byte throttle;
  byte steering;
};
dataPackage data;

//package to contain outgoing acknowledge data
struct ackPackage {
  byte highBatt;
  byte lowBatt;
  byte highTemp1;
  byte lowTemp1;
  byte highTemp2;
  byte lowTemp2;
  byte highIM1;
  byte lowIM1;
  byte highIM2;
  byte lowIM2;
};
ackPackage ack;

void setup() {
  //start wireless connection
  resetACK();
  radio.begin();
  radio.openReadingPipe(0, nRFaddress);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.enableAckPayload();
  radio.writeAckPayload(0, &ack, sizeof(ackPackage));
  radio.startListening();
  resetData();

  //Communciate with roboclaw at 38400 baud
  roboclaw.begin(38400);
  //delay(100);
  roboclaw.ForwardBackwardMixed(rcAddress, data.throttle);
  roboclaw.LeftRightMixed(rcAddress, data.steering);
  //delay(100);
}

void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(dataPackage));
    lastReceiveTime = millis();
    radio.writeAckPayload(0, &ack, sizeof(ackPackage));

    //forward received data roboclaw
    roboclaw.ForwardBackwardMixed(rcAddress, data.throttle);
    roboclaw.LeftRightMixed(rcAddress, data.steering);

    //retreive data from roboclaw for acknowledge payload
    if (readInterval >= readThreshold) {
      readRoboclaw();
      readInterval = 0;
    }
  }

  //verify connection is alive
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000 ) {
    //reset to neutral if connection lost
    resetData();
  }

  readInterval++;
}

void resetData() {
  //Reset the values when there is no radio connection
  data.throttle = 64;
  data.steering = 64;
}

void readRoboclaw() {
  //Read data fromm roboclaw
  //main battery voltage, tenths of a volt
  //bool valid1;
  //rawBattery = roboclaw.ReadMainBatteryVoltage(rcAddress, &valid1);
  rawBattery = roboclaw.ReadMainBatteryVoltage(rcAddress);

  //roboclaw board temperature 1, tenths of a degree
  if (roboclaw.ReadTemp(rcAddress, rawTemp1)) {};

  //roboclaw board temperature 2, tenths of a degree
  if (roboclaw.ReadTemp2(rcAddress, rawTemp2)) {};

  //instantaneous mmotor amp draw, hundredths of an amp
  if (roboclaw.ReadCurrents(rcAddress, rawCurrentMotor1, rawCurrentMotor2)) {};

  //send roboclaw data to acknowledge packet
  resetACK();
}

void resetACK() {
  ack.highBatt = highByte(rawBattery);
  ack.lowBatt = lowByte(rawBattery);
  ack.highTemp1 = highByte(rawTemp1);
  ack.lowTemp1 = lowByte(rawTemp1);
  ack.highTemp2 = highByte(rawTemp2);
  ack.lowTemp2 = lowByte(rawTemp2);
  ack.highIM1 = highByte(rawCurrentMotor1);
  ack.lowIM1 = lowByte(rawCurrentMotor1);
  ack.highIM2 = highByte(rawCurrentMotor2);
  ack.lowIM2 = lowByte(rawCurrentMotor2);
}
