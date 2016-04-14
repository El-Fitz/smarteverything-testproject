#include "ComLink.h"

void    ft_initDownLink (void) {
  downLink.type = 0x00;
  downLink.len = 0xff;
  downLink.msg = NULL;
}

void    ft_resetDownLink (void) {
  downLink.type = 0x00;
  downLink.len = 0xff;
  ft_strfree(downLink.msg);
}

uint8_t ft_isValidType(char c) {
  if (c == 0x21 || c == 0x4b)
    return (1);
  return (0);
}

uint8_t ft_getMsgType(void) {
  if (downLink.type == 0x00 && smeBle.available()) {
    downLink.type = smeBle.read();
    while ((downLink.type == 0xab || downLink.type == 0xad) && smeBle.available())
      downLink.type = smeBle.read();
  }
  if (!ft_isValidType(downLink.type))
    return (0);
  return (1);
}

uint8_t ft_getMsgSize(void) {
  ft_wasteTime(20);
  if (downLink.type == 0x4b)
    downLink.len = KEY_SIZE + 2;
  else if (downLink.type != 0xab && downLink.type != 0xad && smeBle.available()) {
    downLink.len = smeBle.read();
    while (downLink.len == 0xff && smeBle.available())
      downLink.len = smeBle.read();
  }
  if (downLink.len == 0xff)
    return (0);
  return (1);
}

void    ft_getKey(void) {
  for (int i = 0; i < KEY_SIZE; i++)
    downLink.key[i] = downLink.msg[i];
}
