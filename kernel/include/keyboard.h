#ifndef KEYBOARD_H
#define KEYBOARD_H

#define F1    0x80
#define F2    (F1+1)
#define F3    (F2+1)
#define F4    (F3+1)
#define F5    (F4+1)
#define F6    (F5+1)
#define F7    (F6+1)
#define F8    (F7+1)
#define F9    (F8+1)
#define F10   (F9+1)
#define F11   (F10+1)
#define F12   (F11+1)
#define INS   0x8C
#define DEL   (INS+1)
#define HOME  (DEL+1)
#define END   (HOME+1)
#define PGUP  (END+1)
#define PGDN  (PGUP+1)
#define LEFT  0x92
#define UP    (LEFT+1)
#define DOWN  (UP+1)
#define RT    (DOWN+1)
#define SCROL 0x96
#define CAPS  (SCROL+1)
#define NUM   (CAPS+1)
#define SHIFT 0x99
#define ALT   (SHIFT+1)
#define CTRL  (ALT+1)

#define KEY_INS         0x90
#define KEY_DEL         (KEY_INS   + 1)
#define KEY_HOME        (KEY_DEL   + 1)
#define KEY_END         (KEY_HOME  + 1)
#define KEY_PGUP        (KEY_END   + 1)
#define KEY_PGDN        (KEY_PGUP  + 1)
#define KEY_LFT         (KEY_PGDN  + 1)
#define KEY_UP          (KEY_LFT   + 1)
#define KEY_DN          (KEY_UP    + 1)
#define KEY_RT          (KEY_DN    + 1)
#define KEY_PRNT        (KEY_RT    + 1)
#define KEY_PAUSE       (KEY_PRNT  + 1)
#define KEY_LWIN        (KEY_PAUSE + 1)
#define KEY_RWIN        (KEY_LWIN  + 1)
#define KEY_MENU        (KEY_RWIN  + 1)

#define RAW1_LEFT_CTRL       0x1D
#define RAW1_RIGHT_CTRL      0x1D    /* same as left */
#define RAW1_LEFT_SHIFT      0x2A
#define RAW1_RIGHT_SHIFT     0x36
#define RAW1_LEFT_ALT        0x38
#define RAW1_RIGHT_ALT       0x38    /* same as left */
#define RAW1_CAPS_LOCK       0x3A
#define RAW1_F1              0x3B
#define RAW1_F2              0x3C
#define RAW1_F3              0x3D
#define RAW1_F4              0x3E
#define RAW1_F5              0x3F
#define RAW1_F6              0x40
#define RAW1_F7              0x41
#define RAW1_F8              0x42
#define RAW1_F9              0x43
#define RAW1_F10             0x44
#define RAW1_NUM_LOCK        0x45
#define RAW1_SCROLL_LOCK     0x46
#define RAW1_F11             0x57
#define RAW1_F12             0x58

#define KBD_META_ALT         0x0200  // Alt is pressed
#define KBD_META_CTRL        0x0400  // Ctrl is pressed
#define KBD_META_SHIFT       0x0800  // Shift is pressed
#define KBD_META_ANY         (KBD_META_ALT | KBD_META_CTRL | KBD_META_SHIFT)
#define KBD_META_CAPS        0x1000  // CapsLock is on
#define KBD_META_NUM         0x2000  // NumLock is on
#define KBD_META_SCRL        0x4000  // ScrollLock is on

void keyboard_init(void);

int kbd_keys_available(void);
unsigned char kbd_get_key(void);

#endif
