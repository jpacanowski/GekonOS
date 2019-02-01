#ifndef VBE_H
#define VBE_H

#include <types.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_DEPTH 32

#define VBE_DISPI_IOPORT_INDEX  0x01CE
#define VBE_DISPI_IOPORT_DATA   0x01CF

#define VBE_DISPI_INDEX_ID          0x0
#define VBE_DISPI_INDEX_XRES        0x1
#define VBE_DISPI_INDEX_YRES        0x2
#define VBE_DISPI_INDEX_BPP         0x3
#define VBE_DISPI_INDEX_ENABLE      0x4
#define VBE_DISPI_INDEX_BANK        0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH  0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 0x7
#define VBE_DISPI_INDEX_X_OFFSET    0x8
#define VBE_DISPI_INDEX_Y_OFFSET    0x9

#define VBE_DISPI_DISABLED      0x00
#define VBE_DISPI_ENABLED       0x01
#define VBE_DISPI_LFB_ENABLED   0x40
#define VBE_DISPI_VBE_ENABLED   0x40
#define VBE_DISPI_NOCLEARMEM    0x80

#define VBE_DISPI_MAX_XRES  2560
#define VBE_DISPI_MAX_YRES  1600
#define VBE_DISPI_MAX_BPP   32

#define VBE_DISPI_ID0   0xB0C0
#define VBE_DISPI_ID1   0xB0C1
#define VBE_DISPI_ID2   0xB0C2
#define VBE_DISPI_ID3   0xB0C3
#define VBE_DISPI_ID4   0xB0C4
#define VBE_DISPI_ID5   0xB0C5

void VBE_Write(USHORT index, USHORT value);
void VBE_SetVideoMode(USHORT width, USHORT height, USHORT bpp);
void VBE_DrawPixel(UINT x, UINT y, UINT rgbColor);
void VBE_DrawRect(UINT x, UINT y, UINT x2, UINT y2, UINT rgbColor);
void VBE_DrawLine(UINT x1, UINT y1, UINT x2, UINT y2, UINT rgbColor);

#endif
