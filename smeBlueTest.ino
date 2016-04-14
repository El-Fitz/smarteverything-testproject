/*
  smeBlueTest.ino
  Created: 01/04/2016
  Author: Thomas Leger

*/
# include <Wire.h>
# include <Arduino.h>
# include <SME_basic.h>
# include <HTS221.h>
# include <LPS25H.h>
# include <SmeSFX.h>
# include <cc2541.h>
# include "BlueTest.h"

char      bounce = 0;
uint8_t   printed = 0;
long      referenceTime;
long      noAuthRefTime;
bool      sendPayload = false;
long      sendTime;
char      authResponse[1 + (ID_SIZE / 8)];
bool      sigFoxAnswerReady;
bool      sigFoxAnswerAck;
Payload   payload;
Coms      downLink;
Security  safetyFirst;

// the setup function runs once when you press reset or power the board
void setup() {
  SerialUSB.begin(115200);
  Wire.begin();
  smeBle.begin();
  sfxAntenna.begin();
  smeHumidity.begin();
  smePressure.begin();
  ft_initSecurity();
  ft_initDownLink;
  ft_initPayload(NULL);
  randomSeed(analogRead(0));
  for (int wait = 0; !SerialUSB && wait < 60; wait++)
    ft_wasteTime(1000);
  if (!SerialUSB)
    ledYellowOneLight(LOW);
  else
    SerialUSB.println("Nothing will be printed on the console unless asked otherwise");
  ledYellowTwoLight(LOW);
}

// the loop function runs over and over again forever
void loop() 
{
  if (isButtonOnePressed()) {
    safetyFirst.authIsActive = false;
    safetyFirst.idLen = 0;
    noAuthRefTime = millis() / 1000;
  }
  if (isButtonTwoPressed())
    sendPayload == true;
  if (!safetyFirst.authIsActive) {
    ledRedLight(false);
    ledGreenLight(false);
    ledBlueLight(true);
  } else {
    if (!safetyFirst.authenticated) {
      ledRedLight(true);
      ledGreenLight(false);
    }
    ledBlueLight(false);
  }
  if (!safetyFirst.authenticated && safetyFirst.authIsActive)
    ft_establishComLink();
  else if (safetyFirst.authenticated || !safetyFirst.authIsActive)
    ft_getInstruction();
  if (sigFoxAnswerReady = sfxAntenna.hasSfxAnswer())
    sigFoxAnswerAck = true;
  if (safetyFirst.authenticated && safetyFirst.authIsActive && (millis() - 1000 * referenceTime) > SECURITY_RESET_TIME * 1000) {
    referenceTime = millis() / 1000;
    ft_resetSecurity();
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
  } else if (!safetyFirst.authIsActive && (millis() - 1000 * noAuthRefTime) > NO_AUTH_RESET_TIME * 60 * 1000)
    ft_resetSecurity();
  if ((millis() - 1000 * sendTime) > SIGFOX_SEND_TIME * 60 * 1000 && sendPayload == true) {
    ft_sigFoxSendPayload();
    sendPayload = false;
    referenceTime = millis() / 1000;
  }
}
