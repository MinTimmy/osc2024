#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "u_list.h"

/* Lab2 */
void* init_malloc(unsigned int size);
void  init_free(void* ptr);

/* Lab4 */
#define BUDDY_MEMORY_BASE       0x0     // 0x10000000 - 0x20000000 (SPEC) -> Advanced #3 for all memory region
#define BUDDY_MEMORY_PAGE_COUNT 0x3C000 // let BUDDY_MEMORY use 0x0 ~ 0x3C000000 (SPEC)
#define PAGESIZE    0x1000     // 4KB
#define MAX_PAGES   0x10000    // 65536 (Entries), PAGESIZE * MAX_PAGES = 0x10000000 (SPEC)

#define PTR_TO_PAGE_INDEX(ptr) (((unsigned long long)(ptr) - BUDDY_MEMORY_BASE) >> 12)
#define PAGE_INDEX_TO_PTR(idx) ((void *) BUDDY_MEMORY_BASE + (PAGESIZE * (idx)))
#define GET_BUDDY(frame) (&frame_array[frame->idx ^ (1 << frame->val)])


typedef enum {
    FRAME_FREE = -2,
    FRAME_ALLOCATED,
    FRAME_IDX_0 = 0,      //  0x1000
    FRAME_IDX_1,          //  0x2000
    FRAME_IDX_2,          //  0x4000
    FRAME_IDX_3,          //  0x8000
    FRAME_IDX_4,          // 0x10000
    FRAME_IDX_5,          // 0x20000
    FRAME_IDX_FINAL = 6,  // 0x40000
    FRAME_MAX_IDX = 7
} frame_value_type;

typedef enum {
    CACHE_NONE = -1,     // Cache not used
    CACHE_IDX_0 = 0,     //  0x20
    CACHE_IDX_1,         //  0x40
    CACHE_IDX_2,         //  0x80
    CACHE_IDX_3,         // 0x100
    CACHE_IDX_4,         // 0x200
    CACHE_IDX_5,         // 0x400
    CACHE_IDX_FINAL = 6, // 0x800
    CACHE_MAX_IDX = 7
} cache_value_type;

typedef struct frame
{
    struct list_head listhead; // store freelist
    int val;                   // store order
    int used;
    int cache_order;
    unsigned int idx;
} frame_t;

void    init_allocator();
void init_free(void* ptr);
frame_t *release_redundant(frame_t *frame);
int      coalesce(frame_t **frame_ptr);

void dump_page_info(char *msg);
void dump_cache_info(char *msg);

//buddy system
void* page_malloc(unsigned int size);
void  page_free(void *ptr);
void  page2caches(int order);
void* cache_malloc(unsigned int size);
void  cache_free(void* ptr);

void* kmalloc(unsigned int size);
void  kfree(void *ptr);
void  memory_reserve(unsigned long long start, unsigned long long end, char *name);
void _memory_reserve(unsigned long long start, unsigned long long end, int order);

#endif /* _MEMORY_H_ */