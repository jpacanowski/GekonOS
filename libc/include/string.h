#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h>

int strlen(const char *str);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
void append(char s[], char n);
void memset(void *desc, uint8_t val, uint32_t len);
void memcpy(uint8_t *source, uint8_t *dest, int nbytes);

#endif
