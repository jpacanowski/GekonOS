#include <multiboot.h>
#include <video.h>
#include <gdt.h>
#include <idt.h>
#include <timer.h>
#include <keyboard.h>
#include <shell.h>
#include <paging.h>
#include <kheap.h>
#include <mm.h>

void kmain(uint32_t magic, multiboot_info_t *mbi)
{
    UNUSED(magic);
    UNUSED(mbi);

    /* Initialize the video text mode */
    Video_Init();

    /* Set Global Descriptor Table (GDT) */
    GDT_Init();

    /* Set Interrupt Descriptor Table (IDT) */
    IDT_Init();

    /* Initialize Task State Segment (TSS) */

//uint32_t *mem = (uint32_t*)kmalloc(8);
//kprintf("Allocated memory address: %X\n", mem);

    /* Initialize Paging */
    paging_init(mbi->mem_upper);
    
    heap_t *kheap = heap_install();


//(void)kheap;
kprintf("kernel ends at: %x\n", (uint32_t)&end);
kprintf("&kheap: %x kheap->start %x kheap->end %x\n",
                &kheap, kheap->start_address, kheap->end_address);
uint32_t addr = kmalloc_int(8,1,0);
kprintf("&kheap: %x kheap->start %x kheap->end %x\n",
                 &kheap, kheap->start_address, kheap->end_address);
kfree((uint32_t*)addr);
kprintf("&kheap: %x kheap->start %x kheap->end %x\n",
                 &kheap, kheap->start_address, kheap->end_address);




    /* Getting alive... Set interrupts */
    asm volatile("sti");

    /* Initialize the Programmable Interval Timer (PIT) */
    timer_init(50);

    /* Initialize the keyboard */
    keyboard_init();

//uint32_t *mem2 = (uint32_t*)kmalloc(8);
//kprintf("Allocated memory address: %X\n", mem2);
//uint32_t *mem3 = (uint32_t*)kmalloc(8);
//kprintf("Allocated memory address: %X\n", mem3);
    //uint32_t *ptr = (uint32_t*)0xA0000000;
    //uint32_t do_page_fault = *ptr;
    //*ptr = 0xDEDEDEDE;
    //(void)do_page_fault;

    /* Shell */
    lsh_loop();

    /* Remap the Programmable Interrupt Controllers (PICs) to new IDT entries */
    
    /* Initialize Memory Manager */
    //kprintf("Detected memory: %d KB", mbi->mem_upper);
    //MM.Init(mbi);
}
