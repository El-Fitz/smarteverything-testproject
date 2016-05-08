#ifndef BLUETEST_H
# define BLUETEST_H
# include <Wire.h>
# include <Arduino.h>
# include <RTCZero.h>
# include <SME_basic.h>
# include <HTS221.h>
# include <LPS25H.h>
# include <SmeSFX.h>
# include <cc2541.h>
# include "ComLink.h"
# include "Data.h"
# include "Power.h"
# include "Time.h"

void        ft_buttons(void);
void        ft_ledStatus(void);
void        ft_USBputStr(char *str);
void        ft_strfree(char *str);
char        *ft_strnew(size_t size);
char        *ft_strsub(char *s, size_t start, size_t len);
char        *ft_scj(char *s, char c);
void        ft_wasteTime(unsigned int t);

#endif
