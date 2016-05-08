#ifndef TIME_H
# define TIME_H
# include "BlueTest.h"

typedef struct
{
  byte        seconds;
  byte        minutes;
  byte        hours;
  byte        days;
  byte        months;
  byte        years;
}             Time;

void  ft_setRTC(byte seconds, byte minutes, byte hours, byte day, byte months, byte years);
void  ft_timers(void);

#endif
