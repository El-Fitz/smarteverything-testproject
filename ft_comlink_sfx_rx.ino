#include "ComLinkSfx.h"

uint8_t ft_sfxGetStr(void) {
  ft_strfree(payload.answer);
  ft_wasteTime(20);
  for (int i = 0; i < 8; i++) {
    if (SigFox.available()) {
      payload.answer = ft_scj(payload.answer, SigFox.read());
      SerialUSB.print("Answer : ");
      SerialUSB.println(payload.answer);
      i++;
    } else {
      ft_wasteTime(10);
      i++;
    }
  }
  if (!payload.answer)
    return (0);
  return (1);
}

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
