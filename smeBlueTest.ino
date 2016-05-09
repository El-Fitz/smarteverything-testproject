/*
  smeBlueTest.ino
  Created: 01/04/2016
  Author: Thomas Leger
*/

# include "BlueTest.h"
# define SECONDS  0
# define MINUTES  27
# define HOURS    0
# define DAYS     10
# define MONTHS   5
# define YEARS    16
// Flags
volatile uint8_t  interrupted = 0;
volatile uint8_t  canSleep = 0;
uint8_t           printed = 0;
bool              canSendPayload = true;

//Objects & Structures
RTCZero   rtc;
Payload   payload;
Coms      downLink;
Security  safetyFirst;
Time      timer;

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
  ft_setTimer(SECONDS, MINUTES, HOURS, DAYS, MONTHS, YEARS);
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
}

// the loop function runs over and over again forever
void loop() 
{
  ft_updateTime();
  ft_buttons();
  if (!safetyFirst.authenticated && safetyFirst.authIsActive)
    ft_establishComLink();
  else if (safetyFirst.authenticated || !safetyFirst.authIsActive)
    ft_getInstruction();
  ft_ledStatus();
  if (sigFoxAnswerReady = sfxAntenna.hasSfxAnswer())
    sigFoxAnswerAck = true;
  ft_timers();
  if (canSendPayload == true) {
    SerialUSB.println("Sending payload !");
    ft_sigFoxSendPayload();
    canSendPayload = false;
    sendPayload = false;
    timer.payload = millis() / 1000;
  }
  interrupted = 0;
  if (canSleep == 1)
    ft_enterSleep();
}
