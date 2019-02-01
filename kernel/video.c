#include <video.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <types.h>
#include <x86.h>

static USHORT *vidptr;
UCHAR cursor_x = 0;
UCHAR cursor_y = 0;
UCHAR attr = 0;

void Video_Init(void)
{
    vidptr = (USHORT*)0xB8000;
    
    // 0 <- black (background color)
    // 7 <- grey (foreground color)
    attr = (0 << 4) | (7 & 0x0F);
    
    ClearScreen();
    
    kprintf("Initializing Video Text Mode...\n");
}

void ClearScreen(void)
{
    USHORT blank = ' ' | (attr << 8);
    
    for(int i=0; i<(80*25); i++)
    {
        *(vidptr + i) = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    MoveCursor();
}

void ScrollScreen(void)
{
    USHORT blank = ' ' | (attr << 8);
    
    if(cursor_y >= 25)
    {
        for(int i=(0*80); i<(24*80); i++)
        {
            *(vidptr + i) = *(vidptr + (i+80));
        }
        
        for(int i=(24*80); i<(25*80); i++)
        {
            *(vidptr + i) = blank;
        }
        
        cursor_y = 24;
    }
}

void SetColor(char color)
{
    attr = (0 << 4) | (color & 0x0F);
}

void MoveCursor(void)
{
    USHORT CursorLocation = cursor_y * 80 + cursor_x;
    
    /* Tell the VGA board we are setting the high cursor byte */
    outportb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    /* Send the high cursor byte */
    outportb(FB_DATA_PORT, ((CursorLocation >> 8) & 0x00FF));
    /* Tell the VGA board we are setting the low cursor byte */
    outportb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    /* Send the low cursor byte */
    outportb(FB_DATA_PORT, CursorLocation & 0x00FF);
}

void kprintf(const char *str, ...)
{
    int i = 0;
    int pos = 0;
    char buffer[20];
    
    va_list args;
    va_start(args, str);
    
    while(str[i] != '\0')
    {
        switch(str[i])
        {
            case '\n':
                cursor_x = 0;
                cursor_y++;
                break;

            case '\t':
                cursor_x = (cursor_x + 8) & ~(8 - 1);
                break;

            case '%':
                if(str[i+1] == 'd')
                {
                    itoa(va_arg(args, int), buffer, 10);
                    for (int n = 0; buffer[n] != '\0'; n++)
                    {
                        int pos = (cursor_y * 80 + cursor_x);
                        *(vidptr + pos + n) = buffer[n] | (attr << 8);
                    }
                    cursor_x += strlen(buffer);
                    i++;
                }
                else if(str[i+1] == 'x')
                {
                    itoa(va_arg(args, int), buffer, 16);
                    for (int n = 0; buffer[n] != '\0'; n++)
                    {
                        int pos = (cursor_y * 80 + cursor_x);
                        *(vidptr + pos + n) = buffer[n] | (attr << 8);
                    }
                    cursor_x += strlen(buffer);
                    i++;
                }
                else if(str[i+1] == 'X')
                {
                    itoa(va_arg(args, int), buffer, 16);
                    for (int n = 0; buffer[n] != '\0'; n++)
                    {
                        int pos = (cursor_y * 80 + cursor_x);
                        if(n > 1) buffer[n] = toupper(buffer[n]);
                        *(vidptr + pos + n) = buffer[n] | (attr << 8);
                    }
                    cursor_x += strlen(buffer);
                    i++;
                }
                
                break;
                
            default:
                pos = (cursor_y * 80 + cursor_x);
                *(vidptr + pos) = str[i] | (attr << 8);
                cursor_x++;
        }
        
        i++;
        
        if(cursor_x >= 80)
        {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    va_end(args);
    
    ScrollScreen();
    MoveCursor();
}

void kpanic(const char *str, ...)
{
    va_list args;
    va_start(args, str);

    SetColor(RED);

    kprintf("A system error has occured and system has been halted\n");
    kprintf("to prevent damage to your computer. Error information:\n");
    kprintf(str, va_arg(args, int));

    va_end(args);

    /* Halt the CPU */
    __asm__ __volatile__("cli");
    __asm__ __volatile__("hlt");

    /* We need to be sure */
    volatile int i = 0;
    while(true) i++;
}
