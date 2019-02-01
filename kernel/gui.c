#include <vbe.h>
#include <gui.h>

char *mouse_t =
{
    "##00000000\n \
     #1#0000000\n \
     #11#000000\n \
     #111#00000\n \
     #1111#0000\n \
     #11111#000\n \
     #111111#00\n \
     #1111111#0\n \
     #11111111#\n \
     #11111####\n \
     #11#11#000\n \
     #1#0#11#00\n \
     ##00#11#00\n \
     00000#11#0\n \
     00000#11#0\n \
     000000##00\n"
};

void GUI_Init(void)
{
    VBE_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);
    //vbeBuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_DEPTH);
    //vbeBuffer = (UINT *)0xE0000000;

    for(int i=0; i<MAX_WINDOWS; i++)
        window[i].id = 0;

    (void)GUI_CreateWindow(160, 160, 480, 380, "Window 1");
    (void)GUI_CreateWindow(200, 200, 520, 420, "Window 2");
    
    //while(true)
    {
        GUI_DrawDesktop();
        GUI_DrawTaskBar();
        GUI_DrawWindows();
        GUI_DrawMouse();
    }
}

void GUI_DrawDesktop(void)
{
    VBE_DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x5C788C);
}

int GUI_CreateWindow(const int x, const int y,
                    const int w, const int h,
                    const char *title)
{
    int i;
    static int id = 1;

    for(i=0; i<MAX_WINDOWS; i++)
        if(window[i].id == 0)
            break;

    //if(i >= MAX_WINDOWS)
    //    return -1;
    
    /* Static integer variable */
    //static int pos = 40;

    /* Set the ID of the window */
    window[i].id = id;//(i + 1);

    /* Set the position of the window */
    window[i].x = x;//pos;
    window[i].y = y;//pos;

    /* For the future window */
    //pos += 40;

    /* Set the width and height of the window */
    window[i].w = w;
    window[i].h = h;

    /* Set the title of the window */
    (void)title;//strcpy(window[i].title, title);

    /* Set all the windows as not active */
    for(int n=0; n<MAX_WINDOWS; n++)
        window[n].active = false;

    /* Set the current window as active */
    window[i].active = true;

    /* Return the ID of the window */
    return id++;
}

void GUI_DrawWindows(void)
{
    for(int i=0; i<MAX_WINDOWS; i++)
    {
        if(window[i].id != 0)
        {
            int x = window[i].x;
            int y = window[i].y;
            int w = window[i].w;
            int h = window[i].h;
            bool isActive = window[i].active;

            /* Main window */
            VBE_DrawRect(x, y, w, h, 0xD8D8D8);//0xE9E9E8);
    
            /* Title bar */
            VBE_DrawRect(x, y, w, y+22, isActive ? 0x4492D5 : 0x444444);//0x4492D5);

            /* Borders */
            VBE_DrawLine(x, y+22, x, h, 0xC4C5C5); /* Left border */
            VBE_DrawLine(w-1, y+22, w-1, h, 0xC4C5C5); /* Right border */
            VBE_DrawLine(x, h, w-1, h, 0xC4C5C5); /* Bottom border */

            /* Close button */
            //VBE_DrawRect(w-17, y+5, (w-17)+12, (y+5)+12, 0xFF5D5B);
        }
    }
}

void GUI_DrawTaskBar(void)
{
    VBE_DrawRect(0, 0, SCREEN_WIDTH, 25, 0xBDBDBD);
}

void GUI_DrawMouse(void)
{
    int i = 0;
    int x = 1;
    int y = 1;

    do
    {
        if(mouse_t[i] == '1')
        {
            VBE_DrawPixel(x+80, y+80, 0x000000);
            x++;
        }
        else if(mouse_t[i] == '#')
        {
            VBE_DrawPixel(x+80, y+80, 0xFFFFFF);
            x++;
        }
        else if(mouse_t[i] == '0')
        {
            x++;
        }
        else if(mouse_t[i] == '\n')
        {
            x = 1;
            y++;
        }

        i++;
    }
    while(mouse_t[i] != '\0');
}