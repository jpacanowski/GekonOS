#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <types.h>

#define COLUMNS 80
#define LINES 25

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHT_GREY      7
#define DARK_GREY       8
#define LIGHT_BLUE      9
#define LIGHT_GREEN     10
#define LIGHT_CYAN      11
#define LIGHT_RED       12
#define LIGHT_MAGENTA   13
#define LIGHT_BROWN     14
#define WHITE           15

void Video_Init(void);
void MoveCursor(void);
void ClearScreen(void);
void ScrollScreen(void);
void SetColor(char color);
void kprintf(const char *str, ...);
void kpanic(const char *str, ...);

#endif
