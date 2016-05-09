#ifndef DATA_H
# define DATA_H

typedef struct
{
  uint8_t     humidity;
  uint8_t     temp;
  uint8_t     pressure;
  char        str[8];
}             Payload;

void          ft_getData(void);
void          ft_initPayload(char *str);
short int     ft_selectPayload(char c);
void          ft_freePayload(void);
void          ft_printData(uint8_t param);

#endif
