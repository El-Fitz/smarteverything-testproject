#include "ComLink.h"

void  ft_sigFoxConfig(void) {
int initFinish = 1;
    
   sfxAntenna.setSfxConfigurationMode(); // enter in configuration Mode
    do {
        uint8_t answerReady = sfxAntenna.hasSfxAnswer();
        if (answerReady){
            switch (initFinish){
            case 1:                                
                SerialUSB.println("SFX in Data mode");
                sfxAntenna.setSfxDataMode();
                initFinish++;
                break;

            case 2:
                initFinish++; // exit
                break;
            }
        }
    } while (initFinish!=3);
}

void ft_sigFoxSendPayload(void) {
  char  sigFoxPayload[sizeof(payload)];
  int   payloadLen;
  int   strLen;
  
  sigFoxPayload[0] = payload.humidity;
  sigFoxPayload[1] = payload.temp;
  sigFoxPayload[2] = payload.pressure;

  payloadLen = sizeof(payload.str) / sizeof(payload.str[0]);
  strLen = sizeof(sigFoxPayload) / sizeof(sigFoxPayload[0]);
  for (int i = 0; i < payloadLen && i < strLen; i++)
    sigFoxPayload[i + 3] = payload.str[i];
  sfxWakeup();
  ft_wasteTime(20);
  sfxAntenna.sfxSendData(sigFoxPayload, strlen((char*)sigFoxPayload));
  sigFoxAnswerAck = false;
}

void ft_sigFoxAnswerStatus(void) {
  if (sfxAntenna.getSfxMode() == sfxDataMode) {
    switch (sfxAntenna.sfxDataAcknoledge()) {
      case SFX_DATA_ACK_START:
        SerialUSB.println("Waiting Answer");
        break;
      case SFX_DATA_ACK_PROCESSING:
        SerialUSB.print('.');
        break;
      case SFX_DATA_ACK_OK:
        sfxAntenna.setSfxSleepMode(SFX_HW_WAKE);
        break;
      case SFX_DATA_ACK_KO:
        ledRedLight(HIGH);
        SerialUSB.println(' ');
        SerialUSB.println("Answer KO :( :( :( :(");
        break;
        }
  }
}
