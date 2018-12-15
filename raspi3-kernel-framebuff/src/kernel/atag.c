#include <kernel/atag.h>

uint32_t get_mem_size(atag_t * tag) {

  return 1024*1024*128;

   while (tag->tag != NONE) {
       if (tag->tag == MEM) {
           return tag->mem.size;
       }
       tag = (atag_t *)(((uint32_t *)tag) + tag->tag_size);
   }
   return 0;

}
