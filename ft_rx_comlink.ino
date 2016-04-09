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
  char          *test = NULL;

  if (!ft_getStr())
    return ;
  SerialUSB.println("Entering getInstruction");
  SerialUSB.print("Type : ");
  SerialUSB.print(downLink.type, HEX);
  SerialUSB.print("\tInstruction : ");
  ft_USBputStr(downLink.msg);
  if (downLink.type == 0x21 && downLink.msg[0] == safetyFirst.id) {
    if (safetyFirst.nbmsg > 0)
      safetyFirst.nbmsg--;
    smeBle.writeChar(safetyFirst.nbmsg);
    instruction = ft_strsub(downLink.msg, 1, 0);
    smeBle.write(instruction, strlen(instruction));
    for (int i = 0; i < strlen(instruction);) {
      ft_getData();
      switch (instruction[i]) {
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
        default :
          i++;
      }
    }
    if (printed == 1) { 
      SerialUSB.println("");
      printed = 0;
    }
    ft_strfree(instruction);
  }
  ft_strfree(downLink.msg);
}
