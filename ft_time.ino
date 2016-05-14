#include "Time.h"

void  ft_setTimer(byte seconds, byte minutes, byte hours, byte day, byte months, byte years, uint32_t epochTime) {
  rtc.setTime(hours, minutes, seconds);
  timer.seconds = seconds;
  timer.minutes = minutes;
  timer.hours = hours;
  rtc.setDate(day, months, years);
  timer.days = day;
  timer.months = months;
  timer.years = years;
  rtc.setAlarmSeconds(00);
  rtc.setEpoch(epochTime);
  timer.epoch = epochTime;
  rtc.enableAlarm(rtc.MATCH_SS);
  timer.payloadData = 1;
  timer.auth = 0;
  timer.noAuth = 0;
  timer.getTimeSeed = true;
  timer.getData = true;
}

void  ft_synchRTC(void) {
  uint8_t   nulTime;
  uint32_t  newTime;
  uint8_t   newTimeArray[4];

  nulTime = 0;
  if (!sigFoxAnswer)
    return ;
  for (uint8_t i = 0; i < 4; i++) {
    newTimeArray[i] = payload.answer[i];
    if (newTimeArray[i] == 0)
      nulTime += 1;
  }
  if (nulTime == 4)
    payload.receivedTimeSeed = 4;
  else {
    //Something to convert this f*ing array into an uint32_t
    rtc.setEpoch(newTime);
    ft_updateTime();
    payload.receivedTimeSeed = 2;
  }
}

void  ft_updateTime(void) {
  byte  tempSeconds;
  byte  tempMinutes;
  byte  tempDays;
  
  tempSeconds = rtc.getSeconds();
  tempMinutes = rtc.getMinutes();
  timer.hours = rtc.getHours();
  tempDays = rtc.getDay();
  timer.months = rtc.getMonth();
  timer.years = rtc.getYear();
  if (timer.seconds != tempSeconds) {
    //SerialUSB.print("Hours: ");
    //SerialUSB.print(timer.hours);
    //SerialUSB.print("\tMinutes: ");
    //SerialUSB.print(timer.minutes);
    //SerialUSB.print("\tSeconds: ");
    //SerialUSB.println(timer.seconds);
    timer.seconds = tempSeconds;
  }
  if (timer.minutes != tempMinutes) {
    timer.getData = true;
    timer.payloadData += 1;
    if (timer.payloadData > 9) {
     timer.payloadData = 0;
    }
    if (timer.payloadData == 9)
      canSendPayload = true;
    else
      canSendPayload = false;
    timer.minutes = tempMinutes;
  }
  if (timer.days != rtc.getDay()) {
    if (timer.getTimeSeed == false) {
      timer.getTimeSeed = true;
      payload.receivedTimeSeed = 1;
    }
    timer.days = tempDays;
  }
}

void  ft_timers(void) {
  if (timer.getData)
    ft_getData();
  if (safetyFirst.authenticated && (timer.epoch - timer.auth) > SECURITY_RESET_TIME) {
    timer.auth = timer.epoch / 60;
    ft_resetSecurity();
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
  } else if (!safetyFirst.authIsActive && (timer.epoch - timer.noAuth) > NO_AUTH_RESET_TIME * 60)
    ft_resetSecurity();
  if (payload.receivedTimeSeed == 1)
    timer.getTimeSeed == false;
}
