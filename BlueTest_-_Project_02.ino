/*
  SerialCommunication.ino

  Example demonstrating a BLE Bidirectional cummunication between a PC/Smartphone App (Central BLE device)
  and the SmartEveryting (SME) Board (Peripheral BLE)
                    
  - BLE -> sme :    Any character or string written by the Central device, on the writable attribute 0xFFF3, 
                    is processed on the SME board. The board is able to process a full string as either authentication or instructions.
                        String protocol : <len> <type> (optionnal) <id> <remaining len> <char 1> <char 2> ... <char remaining len>
                        Different types: 
                          K, for "Key" (for authentication), 0x4b in hex
                          !, for Instruction (when you send instrcutions to the device), 0x21 in hex
                          
                    The Central device will first have to authenticate itself by sending an authentication message.
                    The authentication message should say how many instructions or messages will be sent by the Central Device.
                    All further messages and instructions will require a new authentication.
                    The key's length is defined by the KEY_SIZE macro in BlueTest.h
                        Authentication protocol : <len> <4b> <nb of msg> <char 1> <char 2> <char 3> ... <char KEY_SIZE>
                        Authentication msg example, for 2 messages, and with a key such as "111111" : 0x094b02010101010101
                    
                    NOTICE: The Host needs to subscribe to 0xFFF4 attribute to get notified of the response.
                    NOTICE: The protocol to be used is <len> <char 1> <char 2> ... <char len>
 
  Created: 01/04/2016

   Author: development

*/

# include "BlueTest.h"

char      bounce = 0;
uint8_t   debug = 1;
uint8_t   printed = 0;
long      referenceTime;
long      sendTime;
Payload   payload;
Coms      downLink;
Security  safetyFirst;


// the setup function runs once when you press reset or power the board
void setup() {
  ft_initSecurity();
  Wire.begin();
  smeHumidity.begin();
  smePressure.begin();
  smeBle.begin();
  SerialUSB.begin(115200);
  while(!SerialUSB);
  //if (debug)
    SerialUSB.println("No character on the console will be sent through the BLE link:");
  ft_initDownLink;
  //if (debug)
    ledYellowOneLight(LOW);
  ledYellowTwoLight(LOW);
  ft_initPayload(NULL);
  randomSeed(analogRead(0));
}

// the loop function runs over and over again forever
void loop() 
{
  if (safetyFirst.id == 0 || safetyFirst.nbmsg <= 0)
    ft_establishComLink();
  if (safetyFirst.id != 0 && safetyFirst.nbmsg != 0)
    ft_getInstruction();
  ft_checkStatus();
  if ((millis() - 1000 * referenceTime) > 30000 && (safetyFirst.id != 0 || safetyFirst.nbmsg != 0)) {
    SerialUSB.print("timer : ");
    SerialUSB.println(millis() - 1000 * referenceTime);
    referenceTime = millis() / 1000;
    //ft_resetSecurity();
  }
}
