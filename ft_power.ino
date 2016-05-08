#include "Power.h"

void ft_alarmInterrupt(void)
{
  interrupted = 1;
  detachInterrupt();
}

void ft_enterSleep(void)
{
  attachInterrupt(ft_alarmInterrupt);
  ft_wasteTime(100);
  rtc.standbyMode();
}
