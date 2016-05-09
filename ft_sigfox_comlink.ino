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

void  ft_sigFoxReceiveAck(void) {
  byte  *answer = NULL;

  if (sfxAntenna.getSfxError() == SME_SFX_OK) {
      answer = (byte *)sfxAntenna.getLastReceivedMessage();
      for (uint8_t i = 0; i < 8; i++)
        payload.answer[i] = (uint8_t)answer[i];
  } else
    sigFoxAnswerAck = false;
}

void  ft_sigFoxSendPayload(void) {
  char  sigFoxPayload[sizeof(payload)];
  byte  *answer;
  int   payloadLen;
  int   strLen;

  sigFoxPayload[0] = (payload.humidity);
  sigFoxPayload[1] = (payload.temp);
  sigFoxPayload[2] = (payload.pressure);
  payloadLen = sizeof(payload.str) / sizeof(payload.str[0]);
  strLen = sizeof(sigFoxPayload) / sizeof(sigFoxPayload[0]);
  for (int i = 0; i < payloadLen && i < strLen; i++)
    sigFoxPayload[i + 3] = payload.str[i];
  sfxWakeup();
  ft_wasteTime(20);
  sfxAntenna.sfxSendDataAck(sigFoxPayload, strlen((char*)sigFoxPayload), timer.getTimeSeed);
  if (timer.getTimeSeed) {
    for (uint8_t i = 0; i < 25 && !sfxAntenna.hasSfxAnswer(); i++)
      ft_wasteTime(i * 1000);
    if (sfxAntenna.hasSfxAnswer() && sfxAntenna.getSfxError() == SME_SFX_OK) {
      answer = (byte *)sfxAntenna.getLastReceivedMessage();
      for (uint8_t i = 0; i < 8; i++)
        payload.answer[i] = (uint8_t)answer[i];
    } else {
      sigFoxAnswerAck = false;
    }
  }
  
  canSendPayload = false;
  sendPayload = false;
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
