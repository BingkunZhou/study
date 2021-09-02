#include "virtual_alloc.h"
#include "virtual_sbrk.h"
#include <stdio.h>
#include <unistd.h> // testing code

void * virtual_heap = NULL;
//void *p = NULL;

void * virtual_sbrk(int32_t increment) {
    // Your implementation here (for your testing only)
    //printf("increment: %d\n",increment);
    //When you use sbrk(0) you get the current "break" address.
    //When you use sbrk(size) you get the previous "break" address
    //printf("increment: %d\n",increment);
    //printf("sbrk(increment): %p\n",sbrk(increment));
    //If the call is successful, virtual_sbrk returns
    //the previous program break of your virtual heap.
    return sbrk(increment); // testing code
    //If the call is unsuccessful
    return (void *)(-1);
}


int main() {
    // Your own testing code here
    //testing code begin
    //printf("virtual_heap: %p\n",virtual_heap);
    virtual_heap = virtual_sbrk(0);
    //printf("virtual_heap: %p\n",virtual_heap);
    init_allocator(virtual_heap, 15, 12);
    virtual_malloc(virtual_heap, 8000);
    //virtual_malloc(virtual_heap, 10000);
    virtual_info(virtual_heap);
    //testing code end
    return 0;
}
