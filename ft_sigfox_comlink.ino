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

void ft_sigFoxSendPayload() {
  sfxWakeup();
  ft_wasteTime(10);
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
