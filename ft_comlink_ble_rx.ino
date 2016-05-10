#include "ComLink.h"

uint8_t ft_getStr(void) {
  ft_resetDownLink();
  if (!ft_getMsgType() || !ft_getMsgSize())
    return (0);
  ft_wasteTime(20);
  downLink.msg = ft_strnew(downLink.len);
  for (int i = 0; i < downLink.len;)
    if (smeBle.available()) {
      downLink.msg[i] = smeBle.read();
      i++;
    }
  if (!downLink.msg)
    return (0);
  return (1);
}

void  ft_getInstruction(void) {
  char          *instruction = NULL;

  if (!ft_getStr())
    return ;
  if (downLink.type == 0x21 && (!safetyFirst.authIsActive || ft_checkID())) {
    timer.auth = millis() / 1000;
    smeBle.writeChar(0x21);
    instruction = ft_strsub(downLink.msg, safetyFirst.idLen, 0);
    smeBle.write(instruction, strlen(instruction));
    for (int i = 0; i < strlen(instruction);) {
      switch (instruction[i]) {
        case 0x70:
          smeBle.writeChar(instruction[i]);
          i += 1;
          break;
        case 0x53:
          ft_bleSendData(instruction[i + 1]);
          i += 2;
          break;
        case 0x50:
          ft_printData(instruction[i + 1]);
          i += 2;
          break;
        case 0x57:
          ft_write(ft_strsub(instruction, i + 2, instruction[i + 1]));
          i += instruction[i + 1] + 2;
          break;
        case 0x44:
          ft_resetSecurity();
          i++;
          break;
        case 0x45:
          SerialUSB.println("instruction: send payload !");
          sendPayload = true;
          i++;
          break;
        default :
          i++;
          break;
      }
    }
    if (printed == 1) { 
      SerialUSB.println("");
      printed = 0;
    }
    ft_strfree(instruction);
  }
  ft_resetDownLink();
}