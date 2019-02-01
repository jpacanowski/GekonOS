#ifndef GUI_H
#define GUI_H

#define MAX_WINDOWS 100

typedef struct
{
    u16 x;
    u16 y;
    u16 w;
    u16 h;
    u16 id;
    char title[80];
    bool active;

    //void (*DrawWindow)();
    //void (*DestroyWindow)();
} window_t;

window_t window[MAX_WINDOWS];

void GUI_Init(void);
void GUI_DrawDesktop(void);
int GUI_CreateWindow(const int x, const int y,
                    const int w, const int h,
                    const char *title);
void GUI_DrawWindows(void);
void GUI_DrawTaskBar(void);
void GUI_DrawMouse(void);

#endif
