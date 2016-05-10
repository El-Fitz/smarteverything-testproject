#include "ComLink.h"

void    ft_initSecurity (void) {
  safetyFirst.authResponse[0] = 0x65;
  for (int i = 0; i < ID_SIZE / 8; i++) {
    safetyFirst.id[i] = 0x00;
    safetyFirst.authResponse[i + 1];
  } 
  for (int i = 0; i < KEY_SIZE; i++)
    safetyFirst.key[i] = 1;
  safetyFirst.maxID = (pow(2, ID_SIZE)) - 1;
  safetyFirst.idLen = ID_SIZE / 8;
  safetyFirst.authIsActive = true;
  safetyFirst.authenticated = false;
  timer.noAuth = 0;
  ledGreenLight(false);
  ledRedLight(true);
}

void    ft_resetSecurity(void) {
  safetyFirst.authResponse[0] = 0x65;
  for (int i = 0; i < safetyFirst.idLen; i++) {
    safetyFirst.id[i] = 0x00;
    safetyFirst.authResponse[i + 1];
  }
  ledRedLight(true);
  ledGreenLight(false);
  safetyFirst.authIsActive = true;
  safetyFirst.authenticated = false;
  timer.noAuth = 0;
  canSleep = 1;
  SerialUSB.println("reseting security");
}

void    ft_updateSeed(void) {
  uint8_t   nulSeed;

  nulSeed = 0;
  if (!sigFoxAnswer)
    return ;
  for (uint8_t i = 0; i < 2; i++) {
    if (payload.answer[i + 4] == 0)
      nulSeed += 1;
  }
  if (!(nulSeed == 2)) {
    for (uint8_t i = 0; i < 2; i++)
      safetyFirst.seed[i] = payload.answer[i + 4];
    if (payload.receivedTimeSeed == 4)
      payload.receivedTimeSeed = 3;
    else
      payload.receivedTimeSeed = 1;
  }
}

void    ft_getIDArray(uint64_t id) {
  uint64_t  mask = 0xFF;
  uint8_t   power = 0;
  
  for (int i = 0; i < safetyFirst.idLen; i++) {
    safetyFirst.id[i] = (mask & id) >> 8 * power;
    power = pow(2, i);
    mask = mask << 8;
  }
}

bool    ft_checkID(void) {
  for (int i = 0; i < downLink.len && i < safetyFirst.idLen; i++)
    if (downLink.msg[i] != safetyFirst.id[i])
      return false;
    return true;
}

void    ft_establishComLink(void) {
  uint64_t id;
  
  if (!ft_getStr())
    return ;
  ft_resetSecurity();
  if (downLink.type == 0x4b && !safetyFirst.authenticated) {
    ft_getKey();
    for (int i = 0; i < KEY_SIZE; i++)
      if (downLink.key[i] != safetyFirst.key[i])
        return ;
    id = random(0, safetyFirst.maxID);
    authResponse[0] = 0x65;
    for (int i = 0; safetyFirst.idLen; i++)
      authResponse[i + 1] = safetyFirst.id[i];
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
    timer.auth = millis() / 1000;
    }
  else {
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
  }
  ft_strfree(downLink.msg);
}
