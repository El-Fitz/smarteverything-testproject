#ifndef BLUETEST_H
# define BLUETEST_H
# include "ComLink.h"
# include "Data.h"

void        ft_USBputStr(char *str);
void        ft_strfree(char *str);
char        *ft_strnew(size_t size);
char        *ft_strsub(char *s, size_t start, size_t len);
char        *ft_scj(char *s, char c);
void        ft_wasteTime(unsigned int t);

#endif
