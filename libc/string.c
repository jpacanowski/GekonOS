#include <types.h>

int strlen(const char s[])
{
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
}

void memset(void *desc, u8 val, u32 len)
{
    u8 *temp = (u8 *)desc;
    for(; len != 0; len--)
        *temp++ = val;
}
