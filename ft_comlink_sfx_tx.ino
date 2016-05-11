#include "ComLinkSfx.h"

static bool   ft_sigFoxSendData(uint8_t *sfxPayload, bool ack) {
  bool  error = false;
  char  sfxCmd[6] = {'A', 'T', '$', 'S', 'F', '='};
  char  sfxEndPayload[3] = {',', '2', ','};

  for (int i = 0; i < 6; i++)
    SigFox.print(sfxCmd[i]);
  for (int i = 0; i < 12; i++) {
    if (sfxPayload[i] < 16) {SigFox.print("0");}
    SigFox.print(sfxPayload[i], HEX);
  }
  for (int i = 0; i < 3; i++)
    SigFox.print(sfxEndPayload[i]);
  if (ack) 
    SigFox.print(1);
  else
    SigFox.print(0);
  while(!SigFox.available()) {
    ft_wasteTime(100);
  }
  if (ft_sfxGetStr() == 1 && payload.answer[0] == 'O')
    error = false;
  else {
    if (payload.answer)
      SerialUSB.println(payload.answer);
    error = true;
  }
  return (!error);
}

void  ft_sigFoxTx(void) {
  uint8_t  sigFoxPayload[12];
  
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
  //sfxAntenna.sfxSendData(sigFoxPayload, 12);
  for (int i = 0; i < 5 && !(ft_sigFoxSendData(sigFoxPayload, timer.getTimeSeed)); i++);
  SerialUSB.print("sfxAntenna.getSfxMode : ");
  SerialUSB.println(sfxAntenna.getSfxMode());
  canSendPayload = false;
  sendPayload = false;
}
