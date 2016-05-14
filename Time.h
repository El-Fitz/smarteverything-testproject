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
  uint32_t    epoch;
  bool        getTimeSeed;
  bool        getData;
  uint8_t     payloadData;
  uint32_t    auth;
  uint32_t    noAuth;
}             Time;

void  ft_setTimer(byte seconds, byte minutes, byte hours, byte day, byte months, byte years);
void  ft_updateTime(void);
void  ft_timers(void);

#endif
