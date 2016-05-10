#ifndef DATA_H
# define DATA_H
# define STRLEN 7
// if neither Time and Seed have been received, receivedTimeSeed is set to 4
// if both Time and Seed have been received, receivedTimeSeed is set to 1
// if only Time has been received, receivedTimeSeed is set to 2
// if only Seed has been received, receivedTimeSeed is set to 3

typedef struct
{
  uint8_t     receivedTimeSeed;
  uint8_t     humidity;
  uint8_t     temp;
  uint8_t     pressure;
  uint8_t     answer[8];
  char        str[STRLEN];
}             Payload;

void          ft_getData(void);
void          ft_initPayload(void);
short int     ft_selectPayload(char c);
void          ft_freePayload(void);
void          ft_printData(uint8_t param);

#endif
