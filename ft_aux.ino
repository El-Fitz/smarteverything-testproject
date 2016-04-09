#include "BlueTest.h"

//When you need to waste some time
void  ft_wasteTime(unsigned int t) {
  for (long int refTime = millis(); millis() - refTime < t;);
}

// Useful function when debugging.
// Prints a char* in hexadecimal on SerialUSB
void    ft_USBputStr(char *str) {
  int len;

  len = strlen(str);
  for (int i = 0; i < len; i++)
    SerialUSB.print(str[i], HEX);
  SerialUSB.println("");
}

void    ft_strfree(char *str) {
  if (str) {
    free(str);
    str = NULL;  
  }
}

char    *ft_strnew(size_t size) {
    size_t  i;
    char    *res;

    i = 0;
    if (!(res = (char *)malloc(size + 1))){
        return (NULL);
    }
    while (i <= size)
    {
        res[i] = '\0';
        i++;
    }
    return (res);
}

char    *ft_strsub(char *s, size_t start, size_t len)
{
  char  *sub;
  int   i = 0;

  if (!s)
    return (NULL);
  s = s + start;
  if (len == 0)
    len = strlen(s);
  if (!(sub = ft_strnew(len)))
    return (NULL);
  while (i < len && (sub[i] = s[i]) && s[i] != '\0')
    i++;
  return (sub);
}

char    *ft_scj(char *s, char c)
{
    int     i;
    char    *res;

    i = 0;
    if (!s && !(s = ft_strnew(0)))
        return (NULL);
    if (!(res = ft_strnew(strlen(s) + 1)))
        return (NULL);
    while (s[i] != '\0' && (res[i] = s[i]))
        i++;
    res[i] = c;
    free(s);
    s = NULL;
    res[i + 1] = '\0';
    return (res);
}
