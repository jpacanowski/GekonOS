#include <kheap.h>
#include <paging.h>
#include <common.h>
#include <video.h>
#include <mm.h>

extern uint32_t placement_address;
extern page_directory_t* kernel_directory;
extern page_directory_t* current_directory;

heap_t* kheap = 0;

heap_t* heap_install(void)
{
    /* Map some pages for the the kernel heap */
    uint32_t i = placement_address, num;
    while(i < (placement_address + 0x10000))
    {
        page_t* page = set_page(i, kernel_directory);
        num = alloc_frame(page, 0, 0);
        kprintf("alloc_frame: %x\n", num * PAGE_SIZE);
        i += PAGE_SIZE;
    }

    // Map some pages in the heap area and create kernel heap there
    for(i=KHEAP_START; i<KHEAP_MAX; i+=PAGE_SIZE)
        alloc_frame(set_page(i, kernel_directory), 0, 0);

    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, KHEAP_MAX, 0, 0);
    return kheap;
}


static void expand(uint32_t new_size, heap_t *heap)
{
    kprintf("expand, new size: %x\n", new_size);
    ASSERT(new_size > (heap->end_address - heap->start_address));

    if((new_size & 0xFFFFF000) != 0) // Get the nearest following page boundary
    {
        new_size &= 0xFFFFF000;
        new_size += PAGE_SIZE;
    }

    ASSERT((heap->start_address + new_size) <= heap->max_address ); // Make sure we are not overreaching ourselves

    uint32_t old_size = heap->end_address - heap->start_address; // This should always be on a page boundary

    uint32_t i = old_size;
    while(i < new_size)
    {
        alloc_frame(get_page(heap->start_address + i, 1, kernel_directory),
                     (heap->supervisor) ? 1 : 0,
                     (heap->readonly) ? 0 : 1
                   );
        i += PAGE_SIZE;
    }
    heap->end_address = heap->start_address + new_size;
}

static uint32_t contract(uint32_t new_size, heap_t *heap)
{
    kprintf("contract, new size: %x\n", new_size);
    ASSERT(new_size < (heap->end_address - heap->start_address));

    if(new_size & PAGE_SIZE) // Get the nearest following page boundary
    {
        new_size &= PAGE_SIZE;
        new_size += PAGE_SIZE;
    }

    if(new_size < HEAP_MIN_SIZE) // Don't contract too far!
        new_size = HEAP_MIN_SIZE;

    uint32_t old_size = heap->end_address - heap->start_address;
    uint32_t i = old_size - PAGE_SIZE;
    while( new_size < i )
    {
        free_frame(get_page(heap->start_address + i, 1, kernel_directory));
        i -= PAGE_SIZE;
    }

    heap->end_address = heap->start_address + new_size;
    return new_size;
}

static uint32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap)
{
    uint32_t iterator = 0;
    while(iterator < heap->index.size)
    {
        header_t *header = (header_t *)lookup_ordered_array(iterator, &heap->index);

        if(page_align > 0)
        {
            uint32_t location = (uint32_t)header; // Page-align the starting point of this header
            long offset = 0;
            if(((location + sizeof(header_t)) & 0xFFFFF000) != 0)
                offset = PAGE_SIZE - (location + sizeof(header_t)) % PAGE_SIZE;
            long hole_size = (long)header->size - offset;
            if(hole_size >= (long)size) // Can we fit now?
                break;
        }
        else if (header->size >= size)
            break;
        ++iterator;
    }
    if(iterator == heap->index.size) // Why did the loop exit?
        return 0xDEADBEEF;//-1; // We got to the end and didn't find anything
    else
        return iterator;
}

static int8_t header_t_less_than(void* a, void* b)
{
    return (((header_t*)a)->size < ((header_t*)b)->size) ? 1 : 0;
}

heap_t* create_heap(uint32_t start_addr, uint32_t end_addr, uint32_t max, uint8_t supervisor, uint8_t readonly)
{
    heap_t* heap = (heap_t*)kmalloc_int(sizeof(heap_t), 1, 0);
    kprintf("k_m of heap_t* heap (size: %d): %x\n", sizeof(heap_t), (uint32_t)heap);

    // All our assumptions are made on startAddress and endAddress being page-aligned
    ASSERT(start_addr % PAGE_SIZE == 0);
    ASSERT(end_addr % PAGE_SIZE == 0);

    // Initialize the index ==> page fault
    heap->index = place_ordered_array((void*)start_addr, HEAP_INDEX_SIZE, &header_t_less_than);
    kprintf("after Initialization of the heap->index: %x\n", (void*)(&heap->index));

    // Shift the start address forward to resemble where we can start putting data
    start_addr += sizeof(type_t) * HEAP_INDEX_SIZE;

    if((start_addr & 0xFFFFF000) != 0) // Make sure the start address is page-aligned
    {
        start_addr &= 0xFFFFF000;
        start_addr += PAGE_SIZE;
    }

    // Write the start, end and max addresses into the heap structure
    heap->start_address = start_addr;
    heap->end_address   = end_addr;
    heap->max_address   = max;
    heap->supervisor    = supervisor;
    heap->readonly      = readonly;

    //TEST
    kprintf("HEAP start: %x end: %x max: %x kernel mode: %d read-only: %d\n",
    heap->start_address, heap->end_address, heap->max_address, heap->supervisor, heap->readonly);
    //TEST

    // We start off with one large hole in the index.
    header_t* hole = (header_t*) start_addr;
    hole->size     =  end_addr - start_addr;
    hole->magic    =  HEAP_MAGIC;
    hole->is_hole  =  1;

    //TEST
    kprintf("hole %x hole-size: %x\n", hole, hole->size);
    //TEST

    insert_ordered_array((void*)hole, &heap->index);
    return heap;
}

void* alloc(uint32_t size, uint8_t page_align, heap_t* heap)
{
    // Make sure we take the size of header/footer into account
    uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
    // Find the smallest hole that will fit
    uint32_t iterator = find_smallest_hole(new_size, page_align, heap);

    if(iterator == 0xDEADBEEF)//-1) // If we didn't find a suitable hole
    {
        // Save some previous data
        uint32_t old_length      = heap->end_address - heap->start_address;
        uint32_t old_end_address = heap->end_address;

        // We need to allocate some more space
        expand(old_length + new_size, heap);
        uint32_t new_length = heap->end_address - heap->start_address;

        // Find the endmost header. (Not endmost in size, but in location)
        iterator = 0;
        // Vars to hold the index of, and value of, the endmost header found so far
        int32_t idx   =  -1;
        uint32_t value =   0;
        while (iterator < heap->index.size)
        {
            uint32_t tmp = (uint32_t)lookup_ordered_array(iterator, &heap->index);
            if (tmp > value)
            {
                value = tmp;
                idx = iterator;
            }
            ++iterator;
        }

        // If we didn't find ANY headers, we need to add one
        if(idx == -1)
        {
            header_t *header  = (header_t *)old_end_address;
            header->magic     = HEAP_MAGIC;
            header->size      = new_length - old_length;
            header->is_hole   = 1;
            footer_t *footer  = (footer_t *) (old_end_address + header->size - sizeof(footer_t));
            footer->magic     = HEAP_MAGIC;
            footer->header    = header;
            insert_ordered_array((void*)header, &heap->index);
        }
        else
        {
            // The last header needs adjusting
            header_t *header = lookup_ordered_array(idx, &heap->index);
            header->size += new_length - old_length;
            // Rewrite the footer.
            footer_t *footer = (footer_t *) ((uint32_t)header + header->size - sizeof(footer_t));
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        // We now have enough space. Recurse, and call the function again
        return alloc(size, page_align, heap);
    }

    header_t *orig_hole_header = (header_t *)lookup_ordered_array(iterator, &heap->index);
    uint32_t orig_hole_pos  = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size = orig_hole_header->size;
    // Here we work out if we should split the hole we found into two parts
    // Is the original hole size - requested hole size less than the overhead for adding a new hole?
    if(orig_hole_size-new_size < (sizeof(header_t) + sizeof(footer_t)))
    {
        // Then just increase the requested size to the size of the hole we found
        size += orig_hole_size - new_size;
        new_size = orig_hole_size;
    }

    // If we need to page-align the data, do it now and make a new hole in front of our block
    if(page_align && (orig_hole_pos & 0xFFFFF000))
    {
        uint32_t new_location    = orig_hole_pos + PAGE_SIZE - (orig_hole_pos & 0xFFF) - sizeof(header_t);
        header_t* hole_header = (header_t*) orig_hole_pos;
        hole_header->size     = PAGE_SIZE - (orig_hole_pos & 0xFFF) - sizeof(header_t);
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        footer_t* hole_footer = (footer_t*) ((uint32_t)new_location - sizeof(footer_t));
        hole_footer->magic    = HEAP_MAGIC;
        hole_footer->header   = hole_header;
        orig_hole_pos         = new_location;
        orig_hole_size        = orig_hole_size - hole_header->size;
    }
    else
    {
        // Else we don't need this hole any more, delete it from the index
        remove_ordered_array(iterator, &heap->index);
    }

    // Overwrite the original header...
    header_t* block_header  = (header_t*) orig_hole_pos;
    block_header->magic     = HEAP_MAGIC;
    block_header->is_hole   = 0;
    block_header->size      = new_size;
    // ... and the footer
    footer_t* block_footer  = (footer_t*) (orig_hole_pos + sizeof(header_t) + size);
    block_footer->magic     = HEAP_MAGIC;
    block_footer->header    = block_header;

    // We may need to write a new hole after the allocated block.
    // We do this only if the new hole would have positive size...
    if(orig_hole_size - new_size > 0)
    {
        header_t* hole_header = (header_t*) (orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
        hole_header->magic    = HEAP_MAGIC;
        hole_header->is_hole  = 1;
        hole_header->size     = orig_hole_size - new_size;
        footer_t* hole_footer = (footer_t*) ((uint32_t)hole_header + orig_hole_size - new_size - sizeof(footer_t));
        if((uint32_t)hole_footer < heap->end_address)
        {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        // Put the new hole in the index
        insert_ordered_array((void*)hole_header, &heap->index);
    }

    // ...And we're done!
    return (void*) ((uint32_t)block_header + sizeof(header_t));
}

void free(void* p, heap_t* heap)
{
    // Exit gracefully for null pointers.
    if(p == 0) return;

    // Get the header and footer associated with this pointer.
    header_t* header = (header_t*) ((uint32_t)p - sizeof(header_t));
    footer_t* footer = (footer_t*) ((uint32_t)header + header->size - sizeof(footer_t));

    // Sanity checks.
    ASSERT(header->magic == HEAP_MAGIC);
    ASSERT(footer->magic == HEAP_MAGIC);

    // Make us a hole.
    header->is_hole = 1;

    // Do we want to add this header into the 'free holes' index?
    char do_add = 1;

    // Unify left, if the thing immediately to the left of us is a footer...
    footer_t* test_footer = (footer_t*) ((uint32_t)header - sizeof(footer_t));
    if((test_footer->magic == HEAP_MAGIC) && (test_footer->header->is_hole == 1))
    {
        uint32_t cache_size = header->size; // Cache our current size.
        header = test_footer->header;     // Rewrite our header with the new one.
        footer->header = header;          // Rewrite our footer to point to the new header.
        header->size += cache_size;       // Change the size.
        do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
    }

    // Unify right
    // If the thing immediately to the right of us is a header...
    header_t* test_header = (header_t*) ((uint32_t)footer + sizeof(footer_t) );
    if((test_header->magic == HEAP_MAGIC) && (test_header->is_hole))
    {
        header->size += test_header->size; // Increase our size.
        test_footer = (footer_t*) ((uint32_t)test_header + // Rewrite it's footer to point to our header.
                                    test_header->size - sizeof(footer_t));
        footer = test_footer;
        // Find and remove this header from the index.
        uint32_t iterator = 0;
        while( ( iterator < heap->index.size) &&
               ( lookup_ordered_array(iterator, &heap->index) != (void*) test_header ) )
            ++iterator;

        // Make sure we actually found the item.
        ASSERT(iterator < heap->index.size);
        // Remove it.
        remove_ordered_array(iterator, &heap->index);
    }

    // If the footer location is the end address, we can contract.
    if(((uint32_t)footer + sizeof(footer_t)) == heap->end_address)
    {
        uint32_t old_length = heap->end_address - heap->start_address;
        uint32_t new_length = contract((uint32_t)header - heap->start_address, heap);
        // Check how big we will be after resizing.
        if (header->size - (old_length - new_length) > 0)
        {
            // We will still exist, resize.
            header->size -= old_length - new_length;
            footer = (footer_t*) ((uint32_t)header + header->size - sizeof(footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            // We will no longer exist. Remove from the index.
            uint32_t iterator = 0;
            while ( (iterator < heap->index.size) &&
                    (lookup_ordered_array(iterator, &heap->index) != (void*)test_header) )
                ++iterator;
            // If we didn't find ourselves, nothing to remove.
            if (iterator < heap->index.size)
                remove_ordered_array(iterator, &heap->index);
        }
    }

    // If required, add us to the index.
    if(do_add == 1)
        insert_ordered_array((void*)header, &heap->index);
}

void kfree(void* p)
{
    free(p, kheap);
}
