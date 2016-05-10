#ifndef COMLINK_H
# define COMLINK_H
# define SEED_SIZE 2            // Daily key generation seed size, in bytes
# define KEY_SIZE 6             // Session key connection size, in bytes
# define ID_SIZE 32             // Random session ID size, in bits
# define SIGFOX_SEND_TIME 10    // SigFox sending interval, IN MINUTES
# define SECURITY_RESET_TIME 1  // Inactivity period for automatic Deauthentication, IN MINUTES
# define NO_AUTH_RESET_TIME 60   // Automatic end of no_auth mode (press button ONE), IN MINUTES

typedef struct
{
  uint8_t   id[ID_SIZE / 8];
  uint64_t  maxID;
  uint8_t   idLen;
  uint8_t   seed[SEED_SIZE];
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
void        ft_updateSeed(void);
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
void        ft_sigFoxRx(void);
void        ft_sigFoxTx(void);
int         ft_bleSendData(char type);

#endif
