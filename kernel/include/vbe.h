#ifndef VBE_H
#define VBE_H

#include <stdint.h>

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

void VBE_Write(uint16_t index, uint16_t value);
void VBE_SetVideoMode(uint16_t width, uint16_t height, uint16_t bpp);
void VBE_DrawPixel(uint32_t x, uint32_t y, uint32_t rgbColor);
void VBE_DrawRect(uint32_t x, uint32_t y, uint32_t x2, uint32_t y2, uint32_t rgbColor);
void VBE_DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t rgbColor);

#endif
