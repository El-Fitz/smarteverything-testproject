// Session connection size, in bytes
// Random session ID, in bits
// SigFox sending interval, IN MINUTES
// Inactivity period for automatic Deauthentication, IN MINUTES
// Automatic end of no_auth mode (press button ONE), IN MINUTES

#ifndef COMLINK_H
# define COMLINK_H
# define KEY_SIZE 6
# define ID_SIZE 32
# define SIGFOX_SEND_TIME 10
# define SECURITY_RESET_TIME 1
# define NO_AUTH_RESET_TIME 60

typedef struct
{
  uint8_t   id[ID_SIZE / 8];
  uint64_t  maxID;
  uint8_t   idLen;
  char      key[KEY_SIZE];
  char      authResponse[1 + (ID_SIZE / 8)];
  bool      authIsActive;
  bool      authenticated;
}           Security;

typedef struct
{
  uint8_t   type;
  uint8_t   len;
  uint8_t   key[KEY_SIZE];
  char      *msg;
}           Coms;

void        ft_sigFoxConfig(void);
void        ft_initSecurity (void);
void        ft_resetSecurity(void);
void        ft_initDownLink (void);
void        ft_resetDownLink (void);
void        ft_getIDArray(uint64_t id);
bool        ft_checkID(void);
uint8_t     ft_isValidType(char c);
uint8_t     ft_getMsgType(void);
uint8_t     ft_getMsgSize(void);
void        ft_getKey(void);
uint8_t     ft_getStr(void);
void        ft_establishComLink(void);
void        ft_getInstruction(void);
void        ft_sigFoxSendPayload(void);
int         ft_bleSendData(char type);

#endif
