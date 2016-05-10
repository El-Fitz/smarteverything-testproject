#include "ComLink.h"

void  ft_sigFoxRx(void) {
  byte  *answer = NULL;
  bool  haveAnswer = false;

  for (int i = 0; i < 25 && !haveAnswer; i++) {
    if (sfxAntenna.hasSfxAnswer())
      haveAnswer = true;
    SerialUSB.print("i : ");
    SerialUSB.println(i, DEC);
    ft_wasteTime(1000);
  }
  if (haveAnswer /*&& sfxAntenna.getSfxError() == SME_SFX_OK*/) {
    SerialUSB.println("Received an answer !");
    answer = (byte *)sfxAntenna.getLastReceivedMessage();
    for (uint8_t i = 0; i < 8; i++)
      payload.answer[i] = (uint8_t)answer[i];
    SerialUSB.print("Received Ack: ");
    ft_USBputStr((char *)payload.answer);
  }
  else {
    SerialUSB.println("Nothing Received");
    sigFoxAnswer = false;
  }
}

void  ft_sigFoxTx(void) {
  char  sigFoxPayload[12];
  
  sigFoxPayload[0] = payload.receivedTimeSeed;
  sigFoxPayload[1] = payload.humidity;
  sigFoxPayload[2] = payload.temp;
  sigFoxPayload[3] = payload.pressure;
  for (int i = 0; i + 4 < 12 && i < STRLEN; i++)
    sigFoxPayload[i + 4] = payload.str[i];
  sfxWakeup();
  ft_wasteTime(100);
  SerialUSB.print("sfxAntenna.getSfxMode : ");
  SerialUSB.print(sfxAntenna.getSfxMode());
  SerialUSB.print("\tgetTimeSeed : ");
  SerialUSB.println(timer.getTimeSeed);
  //sfxAntenna.sfxSendDataAck(sigFoxPayload, 12, timer.getTimeSeed)
  sfxAntenna.sfxSendData(sigFoxPayload, 12);
  SerialUSB.print("sfxAntenna.getSfxMode : ");
  SerialUSB.println(sfxAntenna.getSfxMode());
  canSendPayload = false;
  sendPayload = false;
}
