/*
  smeBlueTest.ino

  Example demonstrating a BLE Bidirectional communication between a PC/Smartphone App (Central BLE device)
  and the SmartEveryting (SME) Board (Peripheral BLE) involving authentication and instructions processing.
                    
  - BLE -> sme :        Any character or string written by the Central device, on the writable attribute 0xFFF3, 
                        is processed on the SME board. The board is able to process a full string as either authentication or instructions.
                            General String protocol : <len> <type> <id> (optionnal) <remaining len> <char 1> <char 2> ... <char remaining len>
                            Different types: 
                              'K', for "Key" (for authentication), 0x4b in hex
                              '!', for "Instruction" (when you send instrcutions to the device), 0x21 in hex
                          
  - Authentication:     The Central device will first have to authenticate itself by sending an authentication message.
                        The authentication message should say how many instructions or messages will be sent by the Central Device.
                        All further messages and instructions will require a new authentication.
                        The key's length is defined by the KEY_SIZE macro in BlueTest.h
                            Authentication protocol : <len> <4b> <nb of msg> <char 1> <char 2> <char 3> ... <char KEY_SIZE>
                            Authentication msg example, for 2 messages, and with a key such as "111111" : 0x094b02010101010101
                            
                        The SME Board will reply to a valid authentication message using the right key,
                        confirming the number of instructions and delivering a random ID between 0 and 255 (0x00 and 0xFF in hex)
                            Authentication reply protocol : <nb of msg><ID>
                            Authentication reply example for the previous authentication msg example, with 42 as the ID : 0x022A
                        
  - Instructions:       The Central device can send instructions to the SME Board using a predefined protocol.
                        The instruction messages allow to perfom the following actions :
                          - Print sensor data on the Serial USB, if available (type 'P' for "Print", 0x50 in hex)
                          - Send sensor data to the Central Bluetooth device (type 'S' for "Send", 0x53 in hex)
                          - Write a string (up to 7 char) into the Payload (type 'W' for "Write", 0x57 in hex)
                        There are multiple sensor data available :
                          - Temperature (parameter 't' for "Temperature", 0x74 in hex)
                          - Humidity (parameter 'h' for "Humidity", 0x68 in hex)
                          - Pressure (parameter 'p' for "Pressure", 0x70 in hex)

                           Instructions protocol : <len> <21> <ID> <remaining len> <type 1> <param 1> <type 2> <param 2> ...
                           Instruction examples :
                              - displaying temperature on SerialUSB, with 0x50 as the ID  : 0x052103505074
                              - sending pressure to Central Device, with 0x1B as the ID   : 0x0521031B5370
                              - writing "Hello !" to payload, with 0xC8 as the ID         : 0x0C210AC8570748656c6c6f2021
                           Instructions can be combined, such as the following :
                              - displaying temperature, writing "Hello !" to payload and sending pressure, with 0xC8 as the ID :
                                  0x0F215074570748656c6c6f20215370
  
  - Instructions reply: The SME Board will reply differently to different instructions:
                        Upon receiving an instruction using the valid ID, the SME board will always reply
                        with the remaining number of instructions allowed for this ID
                              Remaining number of instructions protocol : <remaining number of instructions>
                              Reply example, with 15 remaining messages : 0x0F
                        While processing the instructions, the SME Board will offer differents replies :
                              - Display sensor data on SerialUSB reply protocol   : no reply
                              - Send sensor data to Central Device protocol       : <parameter><value>
                                  Example, with temperature as parameter and 28 as value : 0x741C
                              - Write string to Payload reply protocol            : <written length> followed by <written string>
                                  Example, with "Hello !" as string               : 0x07 followed by 0x48656c6c6f2021
                              
                    NOTICE: In order to be properly stored as an uint8_t, Pressure is truncated.
                            Remember to add +1000 to the value sent to the Central Device
                    NOTICE: The Host needs to subscribe to 0xFFF4 attribute to get notified of the response.
 
  Created: 01/04/2016

   Author: Thomas Leger

*/

# include "BlueTest.h"

char      bounce = 0;
uint8_t   printed = 0;
long      referenceTime;
long      sendTime;
Payload   payload;
Coms      downLink;
Security  safetyFirst;


// the setup function runs once when you press reset or power the board
void setup() {
  
  SerialUSB.begin(115200);
  Wire.begin();
  smeHumidity.begin();
  smePressure.begin();
  smeBle.begin();
  ft_initSecurity();
  ft_initDownLink;
  ft_initPayload(NULL);
  randomSeed(analogRead(0));
  for (int wait = 0; !SerialUSB && wait < 10; wait++)
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
  if (safetyFirst.id == 0 || safetyFirst.nbmsg <= 0)
    ft_establishComLink();
  if (safetyFirst.id != 0 && safetyFirst.nbmsg != 0)
    ft_getInstruction();
  ft_checkStatus();
  if ((millis() - 1000 * referenceTime) > SECURITY_RESET_TIME && (safetyFirst.id != 0 || safetyFirst.nbmsg != 0)) {
    referenceTime = millis() / 1000;
    ft_resetSecurity();
  }
}
