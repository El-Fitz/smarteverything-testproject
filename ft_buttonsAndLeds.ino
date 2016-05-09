#include "BlueTest.h"

void  ft_buttons(void) {
  if (isButtonOnePressed()) {
    safetyFirst.authIsActive = false;
    safetyFirst.idLen = 0;
    timer.noAuth = millis() / 1000;
  }
  if (isButtonTwoPressed())
    sendPayload == true;
}

void  ft_ledStatus(void) {
  if (!safetyFirst.authIsActive) {
    ledRedLight(false);
    ledGreenLight(false);
    ledBlueLight(true);
  } else {
    if (!safetyFirst.authenticated) {
      ledRedLight(true);
      ledGreenLight(false);
    } else {
      ledRedLight(false);
      ledGreenLight(true);
    }
    ledBlueLight(false);
  }
}
