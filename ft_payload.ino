#include "Data.h"

void   ft_initPayload(void) {
  char  str[STRLEN] = {'H', 'e', 'l', 'l', 'o', ' ', '!', '\0'};
  
  payload.receivedTimeSeed = 4;
  payload.humidity = 0;
  payload.temp = 0;
  payload.pressure = 0;
  payload.answer = NULL;
  for (int i = 0; i < STRLEN; i++)
    payload.str[i] = str[i];
  SerialUSB.print("payload.str : ");
  SerialUSB.println(payload.str);
}

short int  ft_selectPayload(char c) {
  if (c == 0x74)
    return(payload.temp);
  else if (c == 0x68)
    return (payload.humidity);
  else if (c == 0x70)
    return (payload.pressure);
}

void      ft_write(char *str)
{
  for (int i = 0; str && (i < 8); i++)
    payload.str[i] = str[i];
  if (str) {
    free(str);
    str = NULL;
  }
  smeBle.writeChar(strlen(payload.str));
  smeBle.write(payload.str, 8);
}

void    ft_freePayload(void) {
  payload.humidity = 0;
  payload.temp = 0;
  for (int i = 0; i < STRLEN; i++)
    payload.str[i] = '\0'; 
}
