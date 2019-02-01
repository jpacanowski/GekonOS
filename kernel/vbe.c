#include <vbe.h>
#include <video.h>
#include <types.h>
#include <math.h>
#include <x86.h>

UINT *vbeBuffer = (UINT *)0xFD000000;

void VBE_Write(USHORT index, USHORT value)
{
    outportw(VBE_DISPI_IOPORT_INDEX, index);
    outportw(VBE_DISPI_IOPORT_DATA, value);
}

void VBE_SetVideoMode(USHORT width, USHORT height, USHORT bpp)
{
    kprintf("Setting Bochs Graphics Adaptor video mode...\n");

    VBE_Write(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    VBE_Write(VBE_DISPI_INDEX_XRES, width);
    VBE_Write(VBE_DISPI_INDEX_YRES, height);
    VBE_Write(VBE_DISPI_INDEX_BPP, bpp);
    VBE_Write(VBE_DISPI_INDEX_ENABLE,
        VBE_DISPI_ENABLED |
        VBE_DISPI_LFB_ENABLED);
}

void VBE_DrawPixel(UINT x, UINT y, UINT rgbColor)
{
    UINT pos = y * SCREEN_WIDTH + x;
    *(vbeBuffer + pos) = rgbColor;
}

void VBE_DrawRect(UINT x, UINT y, UINT x2, UINT y2, UINT rgbColor)
{
    for(UINT ty=y; ty<y2; ty++)
        for(UINT tx=x; tx<x2; tx++)
            VBE_DrawPixel(tx, ty, rgbColor);
}

void VBE_DrawLine(UINT x1, UINT y1, UINT x2, UINT y2, UINT rgbColor)
{
    int sx, sy, dx1=0, dy1, dx2, dy2, x, y, m, n, k=0, cnt;

    sx = x2 - x1;
    sy = y2 - y1;

    if(sx > 0)
        dx1 = 1;
    else if(sx < 0)
        dx1 = -1;
    else
        dy1 = 0;

    if(sy > 0)
        dy1 = 1;
    else if(sy < 0)
        dy1 = -1;
    else
        dy1 = 0;

    m = abs(sx);
    n = abs(sy);

    dx2 = dx1;
    dy2 = 0;

    if(m < n)
    {
        m = abs(sy);
        n = abs(sx);
        dx2 = 0;
        dy2 = dy1;
    }

    x = x1;
    y = y1;

    cnt = m + 1;

    while(cnt--)
    {
        VBE_DrawPixel(x, y, rgbColor);
        k += n;

        if(k < m)
        {
            x += dx2;
            y += dy2;
        }
        else
        {
            k -= m;
            x += dx1;
            y += dy1;
        }
    }
}
