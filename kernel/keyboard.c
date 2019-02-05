#include <isr.h>
#include <x86.h>
#include <video.h>
#include <keyboard.h>
#include <string.h>
#include <queue.h>

#define SC_MAX 57
#define BACKSPACE 0X0E
#define ENTER 0x1C

//static char kbd_buffer[64] = {0};
DECLARE_FIFO_QUEUE(kbd_buffer, 64);

static const unsigned char normal_map[] = {
    /* 00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F   */
    /*00*/ 0,    0x1B, '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
    /*10*/ 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\n', CTRL, 'a',  's',
    /*20*/ 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'', '`',  SHIFT,'#', 'z',  'x',  'c',  'v',
    /*30*/ 'b',  'n',  'm',  ',',  '.',  '/',  SHIFT,0,    ALT,  ' ',  CAPS, F1,   F2,   F3,   F4,   F5,
    /*40*/ F6,   F7,   F8,   F9,   F10,  NUM,  SCROL,HOME, UP,   PGUP, '-',  LEFT, '5',  RT,   '+',  END,
    /*50*/ DOWN, PGDN, INS,  DEL,  0,    0,    '\\', F11,  F12
};

static const unsigned char shift_map[] = {
    /*00*/ 0, 0x1B, '!', '@', '#', '$', '%', '^',
    /*08*/ '&', '*', '(', ')', '_', '+', '\b', '\t',
    /*10*/ 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    /*18*/ 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    /*20*/ 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    /*28*/ '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    /*30*/ 'B', 'N', 'M', '<', '>', '?', 0, 0,
    /*38*/ 0, ' ', 0, F1, F2, F3, F4, F5,
    /*40*/ F6, F7, F8, F9, F10, 0, 0, HOME,
    /*48*/ UP, PGUP, LEFT, '5', RT, '+', END,
    /*50*/ DOWN, PGDN, INS, DEL, 0, 0, 0, F11, F12
};

static void write_kbd(unsigned adr, unsigned data)
{
        unsigned long timeout;
        unsigned stat;

        for(timeout = 500000L; timeout != 0; timeout--)
        {
           stat = inportb(0x64);
           if((stat & 0x02) == 0)
             break;
        }
        if(timeout != 0)
           outportb(adr, data);
}

static void keyboard_callback(registers_t *t)
{
        UNUSED(t);

        // uint8_t scancode = inportb(0x60);

        // if(scancode > SC_MAX)
        //     return;

        // uint8_t key = normal_map[scancode];
        // append(kbd_buffer, key);
        // append(kbd_buffer, '\0');
        // //fifo_insert(&kbd_buffer, key);
        // //kprintf("%c", key);


        unsigned converted;
        unsigned int code = inportb(0x60);

        static unsigned short kbd_status, saw_break_code;
        unsigned short temp;
        if(code >= 0x80)
        {
                saw_break_code = 1;
                code &= 0x7F;
        }
        if(saw_break_code)
        {
                if(code == RAW1_LEFT_ALT || code == RAW1_RIGHT_ALT)
                        kbd_status &= ~KBD_META_ALT;
                else if(code == RAW1_LEFT_CTRL || code == RAW1_RIGHT_CTRL)
                        kbd_status &= ~KBD_META_CTRL;
                else if(code == RAW1_LEFT_SHIFT || code == RAW1_RIGHT_SHIFT)
                        kbd_status &= ~KBD_META_SHIFT;
                saw_break_code = 0;
                return;
        }

        if(code == RAW1_LEFT_ALT || code == RAW1_RIGHT_ALT)
        {
                kbd_status |= KBD_META_ALT;
                return;
        }
        if(code == RAW1_LEFT_CTRL || code == RAW1_RIGHT_CTRL)
        {
                kbd_status |= KBD_META_CTRL;
                return;
        }
        if(code == RAW1_LEFT_SHIFT || code == RAW1_RIGHT_SHIFT)
        {
                kbd_status |= KBD_META_SHIFT;
                return;
        }
        if(code == RAW1_SCROLL_LOCK)
        {
                kbd_status ^= KBD_META_SCRL;
                goto LEDS;
        }
        if(code == RAW1_NUM_LOCK)
        {
                kbd_status ^= KBD_META_NUM;
                goto LEDS;
        }
        if(code == RAW1_CAPS_LOCK)
        {
                kbd_status ^= KBD_META_CAPS;
 LEDS:          write_kbd(0x60, 0xED);
                temp = 0;
                if(kbd_status & KBD_META_SCRL)
                        temp |= 1;
                if(kbd_status & KBD_META_NUM)
                        temp |= 2;
                if(kbd_status & KBD_META_CAPS)
                        temp |= 4;
                write_kbd(0x60, temp);

                // sysprintf("SNC", 64, 0, calc_attrib(7, 4, 0));
                // if(kbd_status & KBD_META_SCRL)
                //         sysprintf("S", 64, 0, calc_attrib(2, 4, 0));
                // if(kbd_status & KBD_META_NUM)
                //         sysprintf("N", 65, 0, calc_attrib(2, 4, 0));
                // if(kbd_status & KBD_META_CAPS)
                //         sysprintf("C", 66, 0, calc_attrib(2, 4, 0));

                return;
        }

        if(kbd_status & KBD_META_SHIFT)
        {
            // ignoruj niepoprawne kody
            if(code >= sizeof(shift_map) / sizeof(shift_map[0]))
             return;
            temp = shift_map[code];
            if(temp == 0)
             return;
            // CapsLock ?
            if(kbd_status & KBD_META_CAPS)
            {
                if(temp>='A' && temp<='Z')
                 temp = normal_map[code];
            }
            converted = shift_map[code];
        } else {
            if(code >= sizeof(normal_map) / sizeof(normal_map[0]))
             return;

            temp = normal_map[code];

            if(temp == 0)
             return;

            if(kbd_status & KBD_META_CAPS)
            {
                if(temp>='a' && temp<='z')
                 temp = shift_map[code];
            }
            converted = normal_map[code];
        }

        if((kbd_status & KBD_META_CTRL) && (kbd_status & KBD_META_ALT) && (temp == DEL))
        {
                kprintf("\nrebooting...");
                //ProcReboot();
        }

        if(code > 0x58)
          return;
        if(converted >= F1 && converted <= F11) {

        } else {
           fifo_insert(&kbd_buffer,converted);
        }

        //return code;
}

void keyboard_init(void)
{
    kprintf("Initializing the keyboard driver...\n\n");
    register_interrupt_handler(IRQ1, keyboard_callback);
}

// uint8_t read_char(void)
// {
//     while(kbd_keys_available()) ;
//     return kbd_get_key();
// }

// char *read_line(void)
// {
//     char c;
//     char *line = "";

//     //while(kbd_keys_available()) ;
//     while((c = kbd_get_key()) != '\n')
//     {
//         //c = kbd_get_key();
//         append(line, c);
//     }

//     //append(line, '\0');
//     return line;
// }

// char *read_line(void)
// {
//     char c;
//     char *line = "";
//     int i = 0;

//     while(kbd_buffer[0] == '\0') ;
//     while((c = kbd_buffer[i]) != '\n')
//     {
//         //c = kbd_get_key();
//         append(line, c);
//         i++;
//     }

//     append(line, '\0');
//     return line;
// }

int kbd_keys_available(void)
{
    int k;
    unsigned long flags;

    __asm__ __volatile__("pushfl\n\t"
                        "popl %0\n\t"
                        "cli":"=r"(flags));
    k = fifo_empty(&kbd_buffer);
    __asm__ __volatile__("pushl %0\n\t"
                        "popfl"::"r"(flags));
    return k;
}

unsigned char kbd_get_key(void)
{
    unsigned long flags;
    unsigned char key = 0;

    __asm__ __volatile__("pushfl\n\t"
                        "popl %0\n\t"
                        "cli":"=r"(flags));
    fifo_get(&kbd_buffer, &key);
    __asm__ __volatile__("pushl %0\n\t"
                        "popfl"::"r"(flags));
    return key;
}

// char *kbd_get_line(void)
// {
//     int i = 0;
//     //unsigned long flags;
//     char *line = "";
//     unsigned char key = 0;

//     while(((key = kbd_buffer[i]) != '\n') && ((key = kbd_buffer[i]) != '\0'))
//     {
//         append(line, key);
//         i++;
//     }
//     kbd_buffer[0] = '\0';
//     return line;
// }
