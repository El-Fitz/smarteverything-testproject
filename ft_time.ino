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
  rightNow.seconds = getSeconds();
  rightNow.minutes = getMinutes();
  rightNow.hours = getHours();
  rightNow.days = getDay();
  rightNow.months = getMonth();
  rightNow.years = getYear();
}

void  ft_timers(void)
{
  ft_getData();
  if (safetyFirst.authenticated && safetyFirst.authIsActive && (millis() - 1000 * referenceTime) > SECURITY_RESET_TIME * 1000) {
    referenceTime = millis() / 1000;
    ft_resetSecurity();
    smeBle.write(authResponse, 1 + (ID_SIZE / 8));
  } else if (!safetyFirst.authIsActive && (millis() - 1000 * noAuthRefTime) > NO_AUTH_RESET_TIME * 60 * 1000)
    ft_resetSecurity();
  if ((millis() - 1000 * payloadSendTime) > SIGFOX_SEND_TIME * 60 * 1000 && sendPayload == true) {
    canSendPayload = true;
  }
}
