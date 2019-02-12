#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

typedef struct
{
    uint32_t present    : 1; /* 0: swapped out, 1: page present in memory */
    uint32_t rw         : 1; /* 0: read-only, 1: read & write */
    uint32_t user       : 1; /* 0: supervisor level, 1: user level */
    uint32_t accessed   : 1; /* Has the page been accessed since last refresh? */
    uint32_t dirty      : 1; /* Has the page been written to since last refresh? */
    uint32_t unused     : 7; /* Combination of unused and reserved bits */
    uint32_t frame_addr : 20; /* Frame address (shifted right 12 bits) */
}
page_t;

typedef struct
{
    page_t pages[1024];
}
page_table_t;

typedef struct
{
    uint32_t tablesPhysical[1024];
    page_table_t *tables[1024];
    uint32_t physicalAddr;
}
page_directory_t;

void paging_init(uint32_t available_mem);
page_t *get_page(uint32_t address, int make, page_directory_t *dir);
page_t *set_page(uint32_t address, page_directory_t *dir);
uint32_t alloc_frame(page_t *page, int is_kernel, int is_writeable);
void free_frame(page_t* page);
void switch_page_directory(page_directory_t *dir);

#endif
