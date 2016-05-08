/*
  smeBlueTest.ino
  Created: 01/04/2016
  Author: Thomas Leger
*/

# include "BlueTest.h"

// Flags
uint8_t   interrupted = 0;
uint8_t   canSleep = 0;
uint8_t   printed = 0;
bool      canSendPayload = true;
bool      sendPayload = false;

//Timers
long      referenceTime;
long      noAuthRefTime;

//Counters
uint8_t   payloadTimer = 0;

//Objects & Structures
RTCZero   rtc;
Payload   payload;
Coms      downLink;
Security  safetyFirst;
Time      rightNow;

//Random ComLink Stuff
char      authResponse[1 + (ID_SIZE / 8)];
bool      sigFoxAnswerReady;
bool      sigFoxAnswerAck;

void ft_initialize(void) {
  SerialUSB.begin(115200);
  Wire.begin();
  sfxAntenna.begin();
  smeBle.begin();
  smeHumidity.begin();
  smePressure.begin();
  rtc.begin();
  ft_initSecurity();
  ft_initDownLink;
  ft_initPayload(NULL);
  ft_setRTC(seconds, minutes, hours, day, months, years)
  randomSeed(analogRead(0));
}

// the setup function runs once when you press reset or power the board
void setup() {
  ft_initialize();
  for (int wait = 0; !SerialUSB && wait < 60; wait++)
    ft_wasteTime(1000);
  if (!SerialUSB)
    ledYellowOneLight(LOW);
  else
    SerialUSB.println("Nothing will be printed on the console unless asked otherwise");
  ledYellowTwoLight(LOW);
  payloadSendTime = 0;
}

// the loop function runs over and over again forever
void loop() 
{ 
  ft_buttons();
  if (!safetyFirst.authenticated && safetyFirst.authIsActive)
    ft_establishComLink();
  else if (safetyFirst.authenticated || !safetyFirst.authIsActive)
    ft_getInstruction();
  ft_ledStatus();
  if (sigFoxAnswerReady = sfxAntenna.hasSfxAnswer())
    sigFoxAnswerAck = true;
  ft_timers();
  if (canSendPayload == true && sendPayload == true) {
    SerialUSB.println("Sending payload !");
    ft_sigFoxSendPayload();
    canSendPayload = false;
    sendPayload = false;
    payloadSendTime = millis() / 1000;
  }
  if (canSleep == 1)
    ft_enterSleep();
}
