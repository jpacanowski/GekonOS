#include <stdint.h>

int strlen(const char s[])
{
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
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
