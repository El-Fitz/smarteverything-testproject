#include "Time.h"

void  ft_setTimer(byte seconds, byte minutes, byte hours, byte day, byte months, byte years) {
  rtc.setTime(hours, minutes, seconds);
  timer.seconds = seconds;
  timer.minutes = minutes;
  timer.hours = hours;
  rtc.setDate(day, months, years);
  timer.days = day;
  timer.months = months;
  timer.years = years;
  rtc.setAlarmSeconds(00);
  rtc.enableAlarm(rtc.MATCH_SS);
  timer.data = 1;
  timer.payload = 0;
  timer.auth = 0;
  timer.noAuth = 0;
  timer.getTime = 0;
  timer.getSeed = 0;
}

void  ft_updateTime(void) {
  if (timer.minutes != rtc.getMinutes()) {
    timer.payload += 1;
    if (timer.payload > 9) {
     timer.payload = 0;
    }
  }
  if (timer.days != rtc.getDay()) {
    if (timer.getTime == 0)
      timer.getTime = 1;
    if (timer.getSeed = 0)
      timer.getSeed = 1;
  }
  timer.seconds = rtc.getSeconds();
  timer.minutes = rtc.getMinutes();
  timer.hours = rtc.getHours();
  timer.days = rtc.getDay();
  timer.months = rtc.getMonth();
  timer.years = rtc.getYear();
  timer.data = timer.minutes % 10 + 1;
}

void  ft_timers(void) {
  ft_getData();
  if (safetyFirst.authenticated && (millis() - 1000 * timer.auth) > SECURITY_RESET_TIME * 1000) {
    timer.auth = millis() / 1000;
    ft_resetSecurity();
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
  } else if (!safetyFirst.authIsActive && (millis() - 1000 * timer.noAuth) > NO_AUTH_RESET_TIME * 60 * 1000)
    ft_resetSecurity();
  if (timer.payload == 9) {
    canSendPayload = true;
  } else {
    canSendPayload = false;
  }
}
