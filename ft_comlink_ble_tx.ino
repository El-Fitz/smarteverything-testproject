#include "ComLink.h"

int        ft_bleSendData(char type) {
  uint8_t x;
  char    res[2];

  x = ft_selectPayload(type);
  if (x <= 0xFF) {
    res[0] = type;
    res[1] = x;
    smeBle.write(res, 2);
    return (1);
   }
  return (0);
}
