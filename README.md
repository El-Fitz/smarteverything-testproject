# smeBlueTest

A small Arduino project demonstrating a BLE Bidirectional communication between a PC/Smartphone App (Central BLE device)
and the SmartEveryting (SME) Board (Peripheral BLE) involving simple authentication and instructions processing.
You can use (for example) use the BlueLight Explorer app for testing purposes.

This project requires the following libraries :
- Arduino.h
- Wire.h
- SME_basic.h
- HTS221.h
- LPS25H.h
- SmeSFX.h
- cc2541.h

The SmartEverything libraries can be found and downloaded here : https://github.com/ameltech.
They can also be downloaded through the Arduino IDE library manager

##BLE -> sme
Any character or string written by the Central device, on the writable attribute 0xFFF3,  is processed on the SME board. The board is able to process a full string as either authentication or instructions.

General String protocol : `<len> <type> <id> (optionnal) <remaining len> <char 1> <char 2> ... <char remaining len>`
	
	Different types:
		'K', for "Key" (for authentication), 0x4b in hex
		'!', for "Instruction" (when you send instrcutions to the device), 0x21 in hex

##Authentication
The Central device will first have to authenticate itself by sending an authentication message.
The authentication message should say how many instructions or messages will be sent by the Central Device.
All further messages and instructions will require a new authentication.
The key's length is defined by the KEY_SIZE macro in BlueTest.h
- Authentication protocol : `<len> <4b> <char 1> <char 2> <char 3> ... <char KEY_SIZE>`
- Authentication msg example, for 2 messages, and with a key such as "111111" : `0x084b010101010101`

The SME Board will reply to a valid authentication message using the right key, using "A" (for "Authentication") as message type, confirming the number of instructions and delivering a random unsigned ID of ID_SIZE bits, defined in ComLink.h
For ID_SIZE == 32, it's between 0 and 4,294,967,295 (0x00 and 0xFFFFFFFF in hex)
- Authentication reply protocol : `<0x65><ID 1/ (ID_SIZE / 8)><ID 2/ (ID_SIZE / 8)><ID 3/ (ID_SIZE / 8)><ID 4/ (ID_SIZE / 8)>`
- Authentication reply example for the previous authentication msg example, with 42 as the ID : `0x650000002A`
- Why (ID_SIZE / 8) ? Because the ID is sent as uint8_t array... so a bytes array. And ID_SIZE is the size in bits.

The SME Board will reply to a valid authentication message using a wrong key
- Authentication reply for valid authentication msg using wrong key : `0x6500000000`

NOTICE: Behaviour has yet to be defined for an ID_SIZE value that isn't a multiple of 8 (full bytes, remember), and a power of 2
NOTICE: Pressing buttonOne will light the RGB LED in Blue and remove the need for authenticaton for a predefined time.
Default time is 60 minutes (1 hour). If authentication is deactivated, do not send an authentication message or an ID in your instruction message. It will just, well... fail.


##Instructions
The Central device can send instructions to the SME Board using a predefined protocol. The instruction messages allow to perfom the following actions :
- Print sensor data on the Serial USB, if available (type 'P' for "Print", 0x50 in hex)
- Send sensor data to the Central Bluetooth device (type 'S' for "Send", 0x53 in hex)
- Send sensor data through SigFox (type 'E' for "Emit", 0x45 in hex)
- Write a string (up to 7 char) into the Payload (type 'W' for "Write", 0x57 in hex)
- Deauthenticate(type 'D' for "Deauthenticate", 0x44 in hex)
- Ping, to maintain "session" (type "p" for "ping", 0x70 in hex)

There are multiple sensor data available :
- Temperature (parameter 't' for "Temperature", 0x74 in hex)
- Humidity (parameter 'h' for "Humidity", 0x68 in hex)
- Pressure (parameter 'p' for "Pressure", 0x70 in hex)

Instructions protocol : `<len> <21> <ID> <remaining len> <type 1> <param 1> <type 2> <param 2> ...`
	
	Instruction examples :
		- displaying temperature on SerialUSB, with 0x50 as the ID  : `0x052103505074`
		- sending pressure to Central Device, with 0x1B as the ID   : `0x0521031B5370`
		- writing "Hello !" to payload, with 0xC8 as the ID         : `0x0C210AC8570748656c6c6f2021`
	
	Instructions can be combined, such as the following :
		- displaying temperature, writing "Hello !" to payload and sending pressure, with 0xC8 as the ID :
			`0x0F215074570748656c6c6f20215370`

##Instructions reply
The SME Board will reply differently to different instructions:
Upon receiving an instruction using the valid ID, the SME board will always reply with a confirmation of the instruction, as well as...
					
While processing the instructions, the SME Board will send differents replies :
- Deauthenticate					:  `0x650000`
- Display sensor data on SerialUSB reply protocol	: `no reply`
- Send sensor data to Central Device protocol		: `<parameter><value>`
	Example, with temp as parameter and 28 as value	: `0x741C`
- Write string to Payload reply protocol	: `<written length>` followed by `<written string>`
	Example, with "Hello !" as string	: `0x07` followed by `0x48656c6c6f2021`

**NOTICE:**	In order to be properly stored as an uint8_t, Pressure is truncated.
		Remember to add +1000 to the value sent to the Central Device
		
**NOTICE:** The Host needs to subscribe to 0xFFF4 attribute to get notified of the response.


##Future Updates to be expected
- DONE: Allow to authenticate withouth key (using button1). Similar to usual Bluetooth pairing
- DONE: Send payload through SigFox network
- WIP : Use RTC to automatically collect sensors' data and send it through SigFox
- WIP : Receive time and T-OTP daily seed (SigFox RSSI ?) through SigFox, once a day
- WIP : Sleep / low power function to save power (using internal RTC, while allowing wake up from Bluetooth interrupt, if possible). Might be activated by default or by pressing button2
- WIP : Connect to SigFox network on startup and request time, in order to update it accordingly for the T-OTP
- Time-Based One-Time-Password Generator for Authentication (from https://github.com/damico/ARDUINO-OATH-TOKEN)
- Encryption of the SigFox payload, most likely through a stream cypher algorithm in order to maintain the payload's size
- Extend encryption to the Sigfox Downlink
