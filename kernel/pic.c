#include <pic.h>
#include <x86.h>

void irq_remap(void)
{
    /* Cascade initialization */
    outportb(PIC1_CTRL, ICW_1); PIC_WAIT();
    outportb(PIC2_CTRL, ICW_1); PIC_WAIT();
    
    /* Remap */
    outportb(PIC1_DATA, IRQ_0); PIC_WAIT();
    outportb(PIC2_DATA, IRQ_8); PIC_WAIT();
    
    /* Cascade identity with slave PIC at IRQ2 */
    outportb(PIC1_DATA, 0x04); PIC_WAIT();
    outportb(PIC2_DATA, 0x02); PIC_WAIT();
    
    /* Request 8086 mode on each PIC */
    outportb(PIC1_DATA, 0x01); PIC_WAIT();
    outportb(PIC2_DATA, 0x01); PIC_WAIT();
    
    /* All done. Null out the data registers */
    outportb(PIC1_DATA, 0x00); PIC_WAIT();
    outportb(PIC2_DATA, 0x00); PIC_WAIT();
}
