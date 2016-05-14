#include "BlueTest.h"

void  ft_buttons(void) {
  if (isButtonOnePressed()) {
    safetyFirst.authIsActive = false;
    safetyFirst.idLen = 0;
    timer.noAuth = timer.epoch;
  }
  if (isButtonTwoPressed())
    ledYellowOneLight(HIGH);
}

void  ft_blinkGreen(void) {
  for (int i = 0; i < 10; i++) {
    ft_wasteTime(100);
    ledGreenLight(HIGH);
    ft_wasteTime(100);
    ledGreenLight(LOW);
  }
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
