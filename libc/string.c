#include <stdint.h>

int strlen(const char *str)
{
    int len = 0;
    while (*str++) len++;
    return len;
}

int strcmp(const char *s1, const char *s2)
{
    while((*s1) && (*s2) && (*s1 == *s2))
        s1++, s2++;
    return (int)*s1 - (int)*s2;
}

char *strcpy(char *dest, const char *src)
{
    char *p = dest;
    while((*dest++ = *src++));
    return p;
}

void append(char s[], char n)
{
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void memset(void *desc, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)desc;
    for(; len != 0; len--)
        *temp++ = val;
}

void memcpy(uint8_t *source, uint8_t *dest, int nbytes)
{
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}
