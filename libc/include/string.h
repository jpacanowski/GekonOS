#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h>

int strlen(const char s[]);
void memset(void *desc, uint8_t val, uint32_t len);
void memcpy(uint8_t *source, uint8_t *dest, int nbytes);

#endif
