#define ICW_1 0x11 /* Enables initialization mode */
#define PIC1_CTRL 0x20 /* I/O base address for master PIC */
#define PIC2_CTRL 0xA0 /* I/O base address for slave PIC */
#define PIC1_DATA 0x21 /* Primary PIC data register */
#define PIC2_DATA 0xA1 /* Secondary PIC data register */
#define IRQ_0 0x20 /* IRQs 0-7 mapped to use interrupts 0x20-0x27 */
#define IRQ_8 0x28 /* IRQs 8-15 mapped to use interrupts 0x28-0x36 */