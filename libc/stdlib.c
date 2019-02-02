#include <stdint.h>

void itoa(int value, char *str, int base)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t divisor = 10;
    uint8_t is_negative = 0;
    uint8_t remainder;
    uint8_t tmp;
    
    /* Check if the base is valid */
    if ((base != 2) && (base != 10) && (base != 16))
    {
        *str = '\0';
        return;
    }
    
    /* Convert 0 */
    if (value == 0)
        str[i++] = '0';
    
    /* Handle negative decimal values */
    if (base == 10 && value < 0)
    {
        is_negative = 1;
        value = -value;
    }
    
    /* Handle hex values */
    else if (base == 16)
        divisor = 16;
    
    /* Convert the value into the corresponding base */
    while (value > 0)
    {
        remainder = value % divisor;
        str[i++] = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        value = value / divisor;
    }
    
    /* Add '-' to negative numbers now, to reverse that later */
    if (base == 10 && is_negative)
        str[i++] = '-';
    
    /* Handle hex values */
    if (base == 16)
    {
        str[i++] = 'x';
        str[i++] = '0';
    }
    
    /* Finalizing by ending the string and reversing it */
    str[i] = '\0';
    for (i = i-1, j = 0; j < i; i--, j++)
    {
        tmp = str[j];
        str[j] = str[i];
        str[i] = tmp;
    }
}