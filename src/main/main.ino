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
//Include libraries
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RoboClaw.h>

//Declare C++ functions to be used
void failsafe_connection_dropped();
void read_rf_data();
void read_roboclaw_data();
void update_acknowledge_package();
void disable_motors();
void enable_motors();

//nRF24 connected to hardware SPI at address 00001
#define CE_PIN 49
#define CSN_PIN 53
RF24 radio(CE_PIN, CSN_PIN);
const byte nRFaddress[6] = "00001";

//Roboclaw connected to Serial1 at address 0x80
RoboClaw roboclaw(&Serial1, 10000);
#define roboclaw_address 0x80
#define motor_stop_pin 7

//Declare variables
uint16_t rawBattery = 0;
uint16_t rawTemp1 = 0;
uint16_t rawTemp2 = 0;
int16_t rawCurrentMotor1 = 0;
int16_t rawCurrentMotor2 = 0;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int readInterval = 0;
int readThreshold = 10;

//Declare flags
bool flag_motor_stop = true;

//package to contain incoming data
struct dataPackage {
  byte throttle;
  byte steering;
  bool data_motor_stop;
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
  bool ack_motor_stop;
};
ackPackage ack;

void setup() {
  pinMode(motor_stop_pin, OUTPUT);

  //start wireless connection
  update_acknowledge_package();
  radio.begin();
  radio.openReadingPipe(0, nRFaddress);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.enableAckPayload();
  radio.writeAckPayload(0, &ack, sizeof(ackPackage));
  radio.startListening();
  
  //Communciate with roboclaw at 38400 baud
  failsafe_connection_dropped();
  roboclaw.begin(38400);
  roboclaw.ForwardBackwardMixed(roboclaw_address, data.throttle);
  roboclaw.LeftRightMixed(roboclaw_address, data.steering);
}

void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    read_rf_data();
  }

  //verify connection is alive
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000 ) {
    //failsafe condition if connection lost
    failsafe_connection_dropped();
  }

  //retreive data from roboclaw for acknowledge payload
  if (readInterval >= readThreshold) {
    read_roboclaw_data();
    readInterval = 0;
  }
  readInterval++;
}

void failsafe_connection_dropped() {
  //Failsafe condition if there is no radio connection
  disable_motors();
  data.throttle = 64;
  data.steering = 64;
}

void read_rf_data() {
  //Read data from nRF module
  radio.read(&data, sizeof(dataPackage));
    lastReceiveTime = millis();
    radio.writeAckPayload(0, &ack, sizeof(ackPackage));

    //Forward received data roboclaw
    roboclaw.ForwardBackwardMixed(roboclaw_address, data.throttle);
    roboclaw.LeftRightMixed(roboclaw_address, data.steering);

    //Check M-Stop flag
    if (flag_motor_stop == true && data.data_motor_stop == false) {
      enable_motors();
    }
    else if (flag_motor_stop == false && data.data_motor_stop == true) {
      disable_motors();
    }
}

void read_roboclaw_data() {
  //Read data fromm roboclaw
  //main battery voltage, tenths of a volt
  bool valid1;
  rawBattery = roboclaw.ReadMainBatteryVoltage(roboclaw_address, &valid1);
  //rawBattery = roboclaw.ReadMainBatteryVoltage(roboclaw_address);

  //roboclaw board temperature 1, tenths of a degree
  if (roboclaw.ReadTemp(roboclaw_address, rawTemp1)) {};

  //roboclaw board temperature 2, tenths of a degree
  if (roboclaw.ReadTemp2(roboclaw_address, rawTemp2)) {};

  //instantaneous mmotor amp draw, hundredths of an amp
  if (roboclaw.ReadCurrents(roboclaw_address, rawCurrentMotor1, rawCurrentMotor2)) {};

  //update acknowledge packet with new data from roboclaw
  update_acknowledge_package();
}

void update_acknowledge_package() {
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
  ack.ack_motor_stop = flag_motor_stop;
}

void disable_motors() {
  digitalWrite(motor_stop_pin, LOW);
  flag_motor_stop = true;
}

void enable_motors() {
  digitalWrite(motor_stop_pin, HIGH);
  flag_motor_stop = false;
}
