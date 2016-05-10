#include "Power.h"

void ft_alarmInterrupt(void)
{
  interrupted = 1;
  rtc.detachInterrupt();
}

void ft_enterSleep(void)
{
  rtc.attachInterrupt(ft_alarmInterrupt);
  ft_wasteTime(100);
  rtc.standbyMode();
}
