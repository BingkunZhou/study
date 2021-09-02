#include "virtual_alloc.h"
#include "virtual_sbrk.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
/*
int8_t    signed char
uint8_t   unsigned char
int16_t   short
uint16_t  unsigned short
int32_t   int
uint32_t  unsigned int
*/
//N=x<<y then N will become N=x*(2^y).
//N=x>>y then N will become N=x/(2^y)
// 分配器的数据结构：
// 第一个字节保存min_size
// 第二个字节保存initial_size-min_size
// 从第三个字节开始一共2的initial_size-min_size次方这么多字节。
// 第i个字节存储的是起点位于i*min_size的连续空闲空间的大小(以min_size为单位)。
// 然后是可以分配的空间
void init_allocator(void *heapstart, uint8_t initial_size, uint8_t min_size) {
    // Your code here
    //printf("init_allocator heapstart: %p initial_size:
    //%d min_size: %d\n",heapstart,initial_size,min_size);//15 12
    //8 * 2^12 = 2^15
    //1 2^12 is a block
    void *p = heapstart;
    //printf("p: %p\n",p); //000
    // 计算当前的堆大小
    // 2 + 4*2^(15-12)+2^15 = 32802
    // 计算需要增加的堆大小
    int32_t increment = 2 + 4*(1 << (initial_size - min_size )) + (1 << initial_size);
    //printf("increment: %d\n",increment);//32802
    //增加堆空间保证能存下2的initial_size次方大小
    if (virtual_sbrk(increment) == (void *)(-1)) {
        printf("virtual_sbrk ERROR");
        return;
    }
    // 保存min_size
    *(uint8_t *)p++ = min_size;
    //printf("p %p = %d\n",p,min_size);  //001 = 12

    //保存initial_size-min_size
    *(uint8_t *)p++ = initial_size - min_size;
    //printf("p %p = %d\n",p,initial_size - min_size); //002 = 3

    //初始时空闲空间的大小是整个空间
    *(int32_t *)p = 1 << (initial_size - min_size);
    //printf("p %p = %d\n",p,1 << (initial_size - min_size)); //002 = 8 2^3

    //移动指针，不能在上一行++
    //006
    p = (int32_t *)p + 1;

    for (size_t i = 1; i < (1 << (initial_size - min_size)); i++){
        *(int32_t *)p = 0;
        // 006 00a 00e 012 016 01a 01e
        //printf("p %p = 0\n",p);
        p = (int32_t *)p + 1;
    }
    return;
}

void *virtual_malloc(void *heapstart, uint32_t size) {
    // Your code here
    //printf("virtual_malloc heapstart: %p size: %d\n",heapstart,size);//000 8000
    if (size == 0) {
        return NULL;
    }

    void *p = heapstart;
    //printf("p: %p\n",p); //000

    uint8_t min_size = *(uint8_t *)p++;
    //printf("min_size at p %p = %d\n",p,min_size);//001 = 12

    uint8_t n = *(uint8_t *)p++;
    //printf("n at p %p = %d\n",p,n);//002 = 3

    uint32_t temp = size >> min_size;
    //printf("temp = %d >> %d %d\n",size,min_size,temp);//8000 >> 12 = 1

    uint32_t l = 0;

    while (temp){
        temp = temp >> 1;
        l++;
    }

    l = 1 << l;
    //判断是不是2的次方
    if ((size & ((size - 1) ^ -1)) == size) {
        l = size >> min_size;
    }

    l = l < 1 ? 1 : l;
    // l = 2 8000 need 2 2^12 block
    //printf("block num: %d\n",l);

    //外层循环，枚举空间长度，最小值是所需的长度，每次翻倍
    //                        2^n      i = i*2  2->4->8
    for (size_t i = l; i <= (1 << n); i = i << 1) {
         //printf("i: %ld\n",i);

        //内层循环，寻找长度为i的空闲空间，如果找到，就分配
        //                      2^n     0->2->4->6  0->4->8
        for (size_t j = 0; j < (1 << n); j += i) {
            //printf("j: %ld\n",j);
            p = (int32_t *)(heapstart + 2) + j;
            //printf("p: %p; p = (int8_t *)heapstart + 2 + j =
            //000 + 2 + %ld =%d\n",p,j,*( (int*) p) );
            // 000 + 2 + 0 = 002 = 8
            // 000 + 2 + 2 = 00a =0
            //找到空闲空间
            if (*(int32_t *)p == i) {
                //i = 8,j=0, p 002 = 8   2^3 =8   8 block of 2^12 in 2^15
                //printf("(*(int32_t *)p == i)\n");
                for (int k = i >> 1; k >= l; k = k >> 1) {
                    //printf("k: %d\n",k);//4 2
                    //p = 002
                    //002 + 4 = 006
                    //将没有分配的空间对应的字节修改为空闲空间的长度
                    *((int32_t *)p + k) = k;
                    //printf("%p = %d\n",((int32_t *)p + k),k);//k =4 012 = 4; k =2 00a = 2
                }
                //分配空间
                *(int32_t *)p = -l;
                //printf("%p = %d\n",p,*( (int*) p));//002 = -2
                //printf("heapstart + 2 + 4 * (1 << n) + j * (1ull << min_size):
                //%p\n",heapstart + 2 + 4 * (1 << n) + j * (1ull << min_size));
                //000 + 2 + 4*(2^3) + 0 *(2^12) = 034 = 022 in hex
                //002 006 00a 00e 012 016 01a 01e 022
                return heapstart + 2 + 4 * (1 << n) + j * (1 << min_size);
            }
        }
    }
    return NULL;
}

int virtual_free(void *heapstart, void *ptr) {
    // Your code here
    //free 8000
    //000 022
    //printf("virtual_free heapstart: %p ptr: %p\n",heapstart,ptr);
    void *p = heapstart;
    uint8_t min_size = *(uint8_t *)p++;
    //printf("min_size at p %p = %d\n",p,min_size);//001 = 12
    uint8_t n = *(uint8_t *)p++;
    //printf("n at p %p = %d\n",p,n);//002 = 3
    //计算指针ptr到分配空间开始的偏移
    int64_t offset = ptr - heapstart - 2 - 4 * (1 << n);
    //printf("offset %ld\n",offset);//0 for 8000   16384 (2^14) for 10000

    //如果偏移小于0或者超过最大空间，则free失败
    if (offset < 0 || offset > 1 << (min_size + n)) {
        return 1;
    }
    //将指针p移到指针ptr所对应的块
    p = heapstart + 2 + 4 * offset / (1 << min_size);
    //printf("%p = %d\n",p,*(int32_t *)p);//002 = -2

    //如果这里存放的数据是非负的，也就是以ptr为起点的区间不是已分配的，则free失败

    if (*(int32_t *)p >= 0) {
        return 1;
    }

    int32_t l = *(int32_t *)p * -1;
    //printf("l = %d\n",l);//2
    //将已分配变为空闲
    *(int32_t *)p = l;
    //printf("%p = %d\n",p,*(int32_t *)p); //002 = 2

    //将长度为i的两个连续空间合并
    for (size_t i = l; i < (1 << n); i = i << 1) {
        //printf("i: %ld\n",i);
        //遍历所有起点
        for (size_t j = 0; j < (1 << n); j += i) {
            //printf("j: %ld\n",j);
            /*
            i: 2
            j: 0
            p: 0x5586d2778002 = 2
            p1: 0x5586d277800a = 2
            */
            p = (int32_t *)(heapstart + 2) + j;
            //printf("p: %p = %d\n",p,*(int32_t *)p);

            void *p1 = (int32_t *)(heapstart + 2) + j + i;
            //printf("p1: %p = %d\n",p1,*(int32_t *)p1);
            //如果有一个长度为i的空闲空间，而且在空间结束的地方又有一个长度为i的空闲空间，
            //亦即两个连续的长度为i的空闲空间，则可以合并
            if (*(int32_t *)p == i && *(int32_t *)p1 == i) {
                //printf("*(int32_t *)p == i && *(int32_t *)p1 == i\n");
                //printf("i = %ld, j = %ld\n",i,j);//i = 2, j = 0;  i= 4, j = 0
                //printf("p: %p = %d\n",p,*(int32_t *)p);
                //printf("p1: %p1 = %d\n",p1,*(int32_t *)p1);
                //将后一段空闲空间的标记清空
                *(int32_t *)p1 = 0;
                //将空闲空间长度*2
                *(int32_t *)p = i << 1;
                //printf("p: %p = %d\n",p,*(int32_t *)p);//8
                //printf("p1: %p1 = %d\n",p1,*(int32_t *)p1);
            }
        }
    }
    return 0;
}

void *virtual_realloc(void *heapstart, void *ptr, uint32_t size) {
    // Your code here
    //printf("virtual_realloc heapstart: %p ptr: %p size: %d\n",heapstart,ptr,size);
    //8000
    //heapstart: 000 ptr: 022 size: 12288 3*2^12
    if (ptr == NULL) {
        return virtual_malloc(heapstart, size);
    }

    if (size == 0) {
        virtual_free(heapstart, ptr);
        return NULL;
    }

    void *p = heapstart;
    //printf("p: %p\n",p); //000

    uint8_t min_size = *(uint8_t *)p++;
    //printf("min_size at p %p = %d\n",p,min_size);//001 = 12

    uint8_t n = *(uint8_t *)p++;
    //printf("n at p %p = %d\n",p,n);//002 = 3

    uint32_t temp = size >> min_size;
    //printf("temp = %d >> %d %d\n",size,min_size,temp);//8000 >> 12 = 1

    uint32_t l = 0;

    while (temp){
        temp = temp >> 1;
        l++;
    }

    l = 1 << l;

    if ((size & ((size - 1) ^ -1)) == size) {
        l = size >> min_size;
    }

    l = l < 1 ? 1 : l;

    if (l > (1 << n)) {
        return NULL;
    }

    virtual_free(heapstart, ptr);

    void *ptr2 = virtual_malloc(heapstart, size);

    //将旧的内容移动到新的地方
    //Copies the values of num bytes from the location
    //pointed by source to the memory block pointed by destination.
    memmove(ptr2, ptr, size);
    //printf("new ptr2: %p\n",ptr2);
    return ptr2;
}

void virtual_info(void *heapstart)
{
    // Your code
    //printf("virtual_info(void *heapstart): %p\n",heapstart);//000
    void *p = heapstart;
    uint8_t min_size = *(uint8_t *)p++;
    //printf("min_size at p %p = %d\n",p,min_size);//001 = 12
    uint8_t n = *(uint8_t *)p++;
    //printf("n at p %p = %d\n",p,n);//002 = 3

    //printf("heapstart + 2 + 4 * (1 << n) %p\n",heapstart + 2 + 4 * (1 << n));
    //002 < 034(022)
    while (p < heapstart + 2 + 4 * (1 << n)) {
        //printf("%p\n",p);
        if (*(int32_t *)p < 0) {
            //printf("%p = %d\n",p,*(int32_t *)p);//002 = -2
            printf("allocated %lld\n", *(int32_t *)p * (1ll << min_size) * -1);
            p += 4 * (*(int32_t *)p * -1);
            //2+2*4 = 10 = a
            //printf("%p\n",p);//00a
        }
        else {
            //00a = 2
            //012 = 4
            printf("free %lld\n", *(int32_t *)p * (1ll << min_size));
            p += 4 * (*(int32_t *)p);
            //printf("%p\n",p);//012
        }
    }
}