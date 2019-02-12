#include <video.h>
#include <string.h>
#include <paging.h>
#include <limits.h>
#include <isr.h>
#include <mm.h>

uint32_t nframes;
uint32_t *frames;

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

extern uint32_t placement_address;

static void page_fault(registers_t *r)
{
    /* A page fault has occurred */
    /* The faulting address is stored in the CR2 register */
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    /* The error code gives us details of what happened */
    int present = !(r->err_code & 0x1); /* Page not present */
    int rw = r->err_code & 0x2; /* Write operation? */
    int us = r->err_code & 0x4; /* Processor was in user-mode? */
    int reserved = r->err_code & 0x8; /* Overwritten CPU-reserved bits of page entry? */
    //int id = r->err_code & 0x10; /* Caused by an instruction fetch? */

    /* Output an error message */
    kprintf("Page fault ( ");
    if(present) kprintf("present ");
    if(rw) kprintf("read-only ");
    if(us) kprintf("user-mode ");
    if(reserved) kprintf("reserved ");
    kprintf(") at %x\n", faulting_address);
    kpanic("PANIC");

    // kpanic("Page fault (%s %s %s %s) at 0x%X",
    //     (present) ? "present " : "XX",
    //     (rw) ? "read-only " : "XX",
    //     (us) ? "user-mode " : "XX",
    //     (reserved) ? "reserved " : "XX",
    //     faulting_address);
}

void paging_init(uint32_t available_mem)
{
    (void)available_mem;kprintf("Initializing Paging...\n");

    nframes = (available_mem * 1024) / PAGE_SIZE;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    kprintf("Detected memory: %dKB (%d frames)\n",
        available_mem, nframes);

    /* Let's make a page directory */
    //kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    //current_directory = kernel_directory;
    uint32_t phys;
    kernel_directory = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    memset(kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physicalAddr = phys;

    /* We need to identity map (phys addr = virt addr) from */
    /* 0x0 to the end of used memory, so we can access this */
    /* transparently, as if paging wasn't enabled. NOTE that */
    /* we use a while loop here deliberately. Inside the loop */
    /* body we actually change placement_address by calling */
    /* kmalloc(). A while loop causes this to be computed */
    /* on-the-fly rather than once at the start. */
    uint32_t i = 0, counter = 0;
    while(i < placement_address)
    {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += PAGE_SIZE;
        counter++;
    }

    kprintf("\n\nINFO FROM PAGING\n");
    kprintf("nframes = %d\n", nframes);
    kprintf("frames = %x\n", frames);
    kprintf("kernel_directory->phys: %x\n", kernel_directory->physicalAddr);
    kprintf("placement_addres: %x\n", placement_address);
    kprintf("allocated frames: %d\n\n", counter);

    /* Before we enable paging, we must register our page fault handler */
    register_interrupt_handler(14, page_fault);

    /* Now, enable paging */
    //switch_page_directory(kernel_directory);
    asm volatile("mov %0, %%cr3" : : "r"(kernel_directory->physicalAddr));
    //uint32_t cr0;
    //asm volatile("mov %%cr0, %0" : "=r"(cr0));
    //cr0 |= 0x80000000; /* Enable paging */
    //asm volatile("mov %0, %%cr0" :: "r"(cr0));

    __asm__ __volatile__("movl %cr0, %eax\n"
                        "orl $0x80000000, %eax\n"
                        "movl %eax, %cr0\n"
                        "jmp 1f\n"
                        "1:\n"
                        "movl $1f, %eax\n"
                        "jmp *%eax\n"
                        "1:");
}

// void switch_page_directory(page_directory_t *dir)
// {
//     current_directory = dir;
//     asm volatile("mov %0, %%cr3" : : "r"(&dir->tablesPhysical));
//     //uint32_t cr0;
//     //asm volatile("mov %%cr0, %0" : "=r"(cr0));
//     //cr0 |= 0x80000000; /* Enable paging */
//     //asm volatile("mov %0, %%cr0" :: "r"(cr0));

//     __asm__ __volatile__("movl %cr0, %eax\n"
//                         "orl $0x80000000, %eax\n"
//                         "movl %eax, %cr0\n"
//                         "jmp 1f\n"
//                         "1:\n"
//                         "movl $1f, %eax\n"
//                         "jmp *%eax\n"
//                         "1:");
// }

static void set_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr / PAGE_SIZE;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] |= (1 << offset);
}

static void clear_frame(uint32_t frame_addr)
{
    uint32_t frame = frame_addr / PAGE_SIZE;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] &= ~(1 << offset);
}

static uint32_t first_free_frame(void)
{
    uint32_t index, offset;

    for(index=0; index<INDEX_FROM_BIT(nframes); index++)
    {
        if(frames[index] != ULONG_MAX) /* nothing free, exit early */
        {
            for(offset=0; offset<32; offset++)
            {
                // uint32_t toTest = 0x1 << j;
                // if(!(frames[i] & toTest))
                // {
                //     return i*4*8+j;
                // }
                if(!(frames[index] & 1<<offset))
                    return index * 32 + offset;
            }
        }
    }
    return ULONG_MAX; /* no free page frames */
}

void free_frame(page_t* page) // deallocate a frame
{
    if(page->frame_addr)
    {
        clear_frame(page->frame_addr);
        page->frame_addr = 0;
    }
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
    (void)make;
    /* turn the address into an index */
    //address /= PAGE_SIZE;
    address >>= 12;// /= PAGE_SIZE;

    /* find the page table containing this address */
    //uint32_t table_index = address / 1024;
    uint32_t table_index = address >> 10;// / 1024;

    // if(dir->tables[table_index])
    // {
    //     return &dir
    //         ->tables[table_index]
    //         ->pages[address % 1024];
    // }
    // else if(make)
    // {
    //     uint32_t temp;
    //     dir->tables[table_index] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &temp);
    //     dir->tablesPhysical[table_index] = temp | 0x7; /* PRESENT, RW, US */
    //     return &dir->tables[table_index]->pages[address % 1024];
    // }
    // else
    // {
    //     return 0;
    // }

    if((((uint32_t)(dir->tablesPhysical[table_index])) & 0x1) && (dir->tables[table_index]))
    {
        //putchar('!');
    }
    else
    {
        uint32_t phys;
        dir->tables[table_index] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &phys);
        memset(dir->tables[table_index], 0, PAGE_SIZE);
        dir->tablesPhysical[table_index] = phys | 0x7; /* PRESENT, RW, US */
    }
    return &dir->tables[table_index]->pages[address % 1024];
}

page_t* set_page(uint32_t address, page_directory_t* dir)
{
    //ULONG tmp = address;
    address >>= 12; // /= PAGESIZE        // index <== address
    uint32_t table_index = (address >> 10);  // ==> page table containing this address

    if(
        ( ( (uint32_t)(  dir->tablesPhysical[table_index] ) ) & 0x1 )
                  && (dir->tables[table_index])
      );
        //putch('!');
    else
    {
        //putch('?'); /*printformat(" address: %x\n",tmp)*/;
        uint32_t phys;
        dir->tables[table_index] = (page_table_t*)kmalloc_int(sizeof(page_table_t), 1, &phys);
        memset(dir->tables[table_index], 0, PAGE_SIZE);
        dir->tablesPhysical[table_index] = phys | 0x7; // 111b meaning: PRESENT=1, RW=1, USER=1
    }
    return &dir->tables[table_index]->pages[address % 1024];
}

uint32_t alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    // if(page->frame_addr != 0)
    //     return;

    // else
    // {
    //     uint32_t index = first_free_frame();

    //     if(index == (uint32_t)-1)
    //     {
    //         /* PANIC! No free frames */
    //     }

    //     set_frame(index * PAGE_SIZE);
    //     page->present = 1;
    //     page->rw = (is_writeable) ? 1 : 0;
    //     page->user = (is_kernel) ? 0 : 1;
    //     page->frame = index;
    // }

    if(!(page->frame_addr))
    {
        uint32_t index = first_free_frame();

        if(index == ULONG_MAX)
        {
            /* PANIC! No free frames */
        }

        set_frame(index * PAGE_SIZE);
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame_addr = index;
        return index;
    }
    return 0;
}
