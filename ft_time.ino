#include "Time.h"

void  ft_setRTC(byte seconds, byte minutes, byte hours, byte day, byte months, byte years)
{
  rtc.setTime(hours, minutes, seconds);
  rightNow.seconds = seconds;
  rightNow.minutes = minutes;
  rightNow.hours = hours;
  rtc.setDate(day, months, years);
  rightNow.days = day;
  rightNow.months = months;
  rightNow.years = years;
  rtc.setAlarmSeconds(00);
  rtc.enableAlarm(rtc.MATCH_SS);
}

void  ft_updateTime()
{
  rightNow.seconds = rtc.getSeconds();
  rightNow.minutes = rtc.getMinutes();
  rightNow.hours = rtc.getHours();
  rightNow.days = rtc.getDay();
  rightNow.months = rtc.getMonth();
  rightNow.years = rtc.getYear();
}

void  ft_timers(void)
{
  ft_getData();
  if (safetyFirst.authenticated && (millis() - 1000 * referenceTime) > SECURITY_RESET_TIME * 1000) {
    referenceTime = millis() / 1000;
    ft_resetSecurity();
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
  } else if (!safetyFirst.authIsActive && (millis() - 1000 * noAuthRefTime) > NO_AUTH_RESET_TIME * 60 * 1000)
    ft_resetSecurity();
  if (rightNow.minutes % 10 == 0) {
    canSendPayload = true;
  } else {
    canSendPayload = false;
  }
}
