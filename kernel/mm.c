#include <mm.h>
#include <paging.h>
#include <video.h>
#include <kheap.h>

extern heap_t *kheap;
extern page_directory_t *kernel_directory;

uint32_t placement_address = 0x200000;// (uint32_t)&end;

uint32_t kmalloc_int(uint32_t size, uint8_t align, uint32_t *phys)
{
    if(kheap != 0)
    {
        void *addr = alloc(size, align, kheap);

        if(phys != 0)
        {
            page_t *page = get_page((uint32_t)addr, 1, kernel_directory);
            *phys = page->frame_addr*PAGE_SIZE+((uint32_t)addr&0x00000FFF);
        }

        kprintf("kmalloc heap: %X\n", (uint32_t)addr);
        return (uint32_t)addr;
    }

    /* For now, we just assign memory at placement_address */
    /* and increment it by size. Even when we have coded our kernel */
    /* heap, this will be useful for use before the heap is initialized */

    if(align == 1 && (placement_address & 0xFFFFF000))
    {
        /* align the placement address */
        placement_address &= 0xFFFFF000;
        placement_address += PAGE_SIZE;
    }

    if(phys)
    {
        *phys = placement_address;
    }

    uint32_t temp = placement_address;
    placement_address += size;
    return temp;
}

uint32_t kmalloc_a(uint32_t size)
{
    return kmalloc_int(size, 1, 0);
}

uint32_t kmalloc_p(uint32_t size, uint32_t *phys)
{
    return kmalloc_int(size, 0, phys);
}

uint32_t kmalloc_ap(uint32_t size, uint32_t *phys)
{
    return kmalloc_int(size, 1, phys);
}

uint32_t kmalloc(uint32_t size)
{
    return kmalloc_int(size, 0, 0);
}
