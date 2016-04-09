#ifndef COMLINK_H
# define COMLINK_H
# define KEY_SIZE 6
# define SECURITY_RESET_TIME 60000

# include "BlueTest.h"

typedef struct
{
  uint8_t   id;
  uint8_t   nbmsg;
  char      key[KEY_SIZE];
}           Security;

typedef struct
{
  uint8_t   type;
  uint8_t   len;
  char      *msg;
}           Coms;

void        ft_initSecurity (void);
void        ft_initDownLink (void);
void        ft_resetDownLink (void);
uint8_t     ft_isValidType(char c);
uint8_t     ft_getMsgType(void);
uint8_t     ft_getMsgSize(void);
uint8_t     ft_getStr(void);
void        ft_establishComLink(void);
void        ft_getInstruction(void);
int         ft_bleSendData(char type);

#endif
