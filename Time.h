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
  bool        getTimeSeed;
  bool        getData;
  uint8_t     payloadData;
  uint8_t     auth;
  uint8_t     noAuth;
}             Time;

void  ft_setTimer(byte seconds, byte minutes, byte hours, byte day, byte months, byte years);
void  ft_updateTime(void);
void  ft_timers(void);

#endif
