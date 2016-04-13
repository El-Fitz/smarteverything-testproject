#include "ComLink.h"

void    ft_initSecurity (void) {
  safetyFirst.id = 0;
  safetyFirst.nbmsg = 0;
  for (int i = 0; i < KEY_SIZE; i++)
    safetyFirst.key[i] = 1;
  ledGreenLight(false);
  ledRedLight(true);
}

void    ft_resetSecurity(void) {
  safetyFirst.id = 0;
  safetyFirst.nbmsg = 0;
  ledRedLight(true);
  ledGreenLight(false);
}

void    ft_checkStatus (void) {
  if (safetyFirst.nbmsg <= 0)
    ft_initSecurity();
}

void    ft_establishComLink(void) {
  char    *key = NULL;
  uint8_t id;

  ft_resetSecurity();
  if (!ft_getStr())
    return ;
  if (downLink.type == 0x4b && !safetyFirst.id) {
    key = ft_strsub(downLink.msg, 1, KEY_SIZE);
    if (strlen(key) != KEY_SIZE)
      return ;
    for (int i = 0; i < KEY_SIZE; i++)
      if (key[i] != safetyFirst.key[i])
        return ;
    safetyFirst.nbmsg = downLink.msg[0];
    safetyFirst.id = random(0, 256);
    authResponse[0] = 0x65;
    authResponse[1] = safetyFirst.nbmsg;
    authResponse[2] = safetyFirst.id;
    smeBle.write(authResponse, 3);
    ledRedLight(false);
    ledGreenLight(true);
    referenceTime = millis() / 1000;
    }
  else {
    authResponse[0] = 0x65;
    authResponse[1] = 0x00;
    authResponse[2] = 0x00;
    smeBle.write(authResponse, 3);
  }
  ft_strfree(downLink.msg);
  ft_strfree(key);
}
