#include "Power.h"

void ft_alarmInterrupt(void)
{
  interrupted = 1;
  detachInterrupt(0);
}

void ft_enterSleep(void)
{
  attachInterrupt(0, ft_alarmInterrupt, LOW);
  ft_wasteTime(100);
  rtc.standbyMode();
}
